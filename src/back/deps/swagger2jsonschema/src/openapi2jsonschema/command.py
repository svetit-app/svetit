#!/usr/bin/env python

import copy
import json
import os
import sys
import urllib

import click
import jsonref  # type: ignore
import jsonschema  # type: ignore
import yaml

from .definitions import kubernetes_definitions
from .errors import UnsupportedError
from .log import debug, error, info
from .util import (
    additional_properties,
    allow_null_optional_fields,
    append_no_duplicates,
    change_dict_values,
    get_request_and_response_body_components_from_paths,
    get_request_parameters_from_paths,
    parse_headers,
    replace_int_or_string,
)


@click.command()
@click.option(
    "-o",
    "--output",
    default="schemas",
    metavar="PATH",
    help="Directory to store schema files",
)
@click.option(
    "-p",
    "--prefix",
    default="_definitions.json",
    help="Prefix for JSON references (only for OpenAPI versions before 3.0)",
)
@click.option(
    "-H",
    "--header",
    multiple=True,
    help="Extra header to use when getting a schema. May be specified multiple times.",
)
@click.option(
    "--stand-alone", is_flag=True, help="Whether or not to de-reference JSON schemas"
)
@click.option(
    "--expanded", is_flag=True, help="Expand Kubernetes schemas by API version"
)
@click.option(
    "--kubernetes", is_flag=True, help="Enable Kubernetes specific processors"
)
@click.option(
    "--strict",
    is_flag=True,
    help="Prohibits properties not in the schema (additionalProperties: false)",
)
@click.option(
    "--include-bodies",
    is_flag=True,
    help="Include request and response bodies as if they are components",
)
@click.option(
    "--include-parameters",
    is_flag=True,
    help="Include request parameters",
)
@click.argument("schema", metavar="SCHEMA_URL")
def default(
    output,
    schema,
    prefix,
    header,
    stand_alone,
    expanded,
    kubernetes,
    strict,
    include_bodies,
    include_parameters,
):
    json_encoder = json.JSONEncoder(
        skipkeys=False,
        ensure_ascii=True,
        check_circular=True,
        allow_nan=False,
        sort_keys=True,
        indent=2,
    )

    json_validator = jsonschema.Draft7Validator

    """
    Converts a valid OpenAPI specification into a set of JSON Schema files
    """
    info("Downloading schema")
    if os.path.isfile(schema):
        schema = "file:///" + os.path.realpath(schema)
    if header:
        headers = parse_headers(header)
        req = urllib.request.Request(schema, headers=headers)
    else:
        req = urllib.request.Request(schema)
    response = urllib.request.urlopen(req)

    info("Parsing schema")
    body = response.read()
    try:
        data = json.loads(body)
    except ValueError as e:
        data = yaml.load(body, Loader=yaml.SafeLoader)

    if "swagger" in data:
        spec_version = data["swagger"]
    elif "openapi" in data:
        spec_version = data["openapi"]
    else:
        raise ValueError(
            "Cannot convert data to JSON because we could not find 'openapi' or 'swagger' keys"
        )

    if not os.path.exists(output):
        os.makedirs(output)

    if spec_version < "3":
        with open(
            "%s/_definitions.json" % output, "w", newline="\n"
        ) as definitions_file:
            info("Generating shared definitions")
            definitions = data["definitions"]
            if kubernetes:
                # Add Kubernetes specific definitions
                definitions.update(copy.deepcopy(kubernetes_definitions))
                for type_name in definitions:
                    type_def = definitions[type_name]
                    # Skip non-object types
                    if not isinstance(type_def.get("properties"), dict):
                        continue
                    # For Kubernetes, populate `apiVersion` and `kind` properties from `x-kubernetes-group-version-kind`
                    has_group_version_kind = (
                        "x-kubernetes-group-version-kind" in type_def
                    )
                    for prop_name in type_def["properties"]:
                        if prop_name == "apiVersion":
                            if has_group_version_kind and expanded:
                                for kube_ext in type_def[
                                    "x-kubernetes-group-version-kind"
                                ]:
                                    api_version = (
                                        kube_ext["group"] + "/" + kube_ext["version"]
                                        if kube_ext["group"]
                                        else kube_ext["version"]
                                    )
                                    append_no_duplicates(
                                        type_def["properties"]["apiVersion"],
                                        "enum",
                                        api_version,
                                    )
                        if prop_name == "kind":
                            if has_group_version_kind:
                                for kube_ext in type_def[
                                    "x-kubernetes-group-version-kind"
                                ]:
                                    kind = kube_ext["kind"]
                                    append_no_duplicates(
                                        type_def["properties"]["kind"],
                                        "enum",
                                        kind,
                                    )
                        # Enum values in properties should be unique
                        if "enum" in type_def["properties"][prop_name]:
                            type_def["properties"][prop_name]["enum"] = list(
                                dict.fromkeys(type_def["properties"][prop_name]["enum"])
                            )
            if strict:
                definitions = additional_properties(definitions)
            print(
                json_encoder.encode({"definitions": definitions}),
                file=definitions_file,
            )

    types = []

    components = {}
    info("Generating individual schemas")
    if spec_version < "3":
        components = data["definitions"]
    else:
        if "components" in data:
            components = data["components"]["schemas"]

    if include_bodies:
        components.update(
            get_request_and_response_body_components_from_paths(data["paths"]),
        )

    if include_parameters:
        debug("Processing request parameters")
        components.update(get_request_parameters_from_paths(data["paths"]))

    for title in components:
        kind = title.split(".")[-1].lower()
        if kubernetes:
            # If it is a Kubernetes object, use a simplified group name
            if title.startswith("io.k8s"):
                group = title.split(".")[-3].lower()
            # Otherwise, use the fully qualified name as the group name
            else:
                group = ".".join(title.split(".")[:-2][::-1]).lower()
            version = title.split(".")[-2].lower()

        specification = components[title]
        specification["$schema"] = json_validator.META_SCHEMA["$schema"]
        specification.setdefault("type", "object")

        if strict:
            specification["additionalProperties"] = False

        if kubernetes and expanded:
            if group in ["core", "api"]:
                full_name = "%s-%s" % (kind, version)
            else:
                full_name = "%s-%s-%s" % (kind, group, version)
        else:
            full_name = kind

        types.append(title)

        try:
            debug("Processing %s" % full_name)

            # These APIs are all deprecated
            if kubernetes:
                if title.split(".")[3] == "pkg" and title.split(".")[2] == "kubernetes":
                    raise UnsupportedError(
                        "%s not currently supported, due to use of pkg namespace"
                        % title
                    )

            updated = change_dict_values(specification, prefix, spec_version)
            specification = updated

            if stand_alone:
                base = "file:///%s/" % os.path.realpath(output)
                specification = jsonref.replace_refs(specification, base_uri=base)

            if "additionalProperties" in specification:
                if specification["additionalProperties"]:
                    updated = change_dict_values(
                        specification["additionalProperties"], prefix, spec_version
                    )
                    specification["additionalProperties"] = updated

            if strict and "properties" in specification:
                updated = additional_properties(specification["properties"])
                specification["properties"] = updated

            if kubernetes and "properties" in specification:
                updated = replace_int_or_string(specification["properties"])
                updated = allow_null_optional_fields(updated)
                specification["properties"] = updated

            json_validator.check_schema(specification)

            with open(
                "%s/%s.json" % (output, full_name), "w", newline="\n"
            ) as schema_file:
                debug("Generating %s.json" % full_name)
                print(
                    json_encoder.encode(specification),
                    file=schema_file,
                )
        except Exception as e:
            error("An error occured processing %s: %s" % (kind, e))
            sys.exit(1)

    with open("%s/all.json" % output, "w", newline="\n") as all_file:
        info("Generating schema for all types")
        contents = {"oneOf": []}
        for title in types:
            if spec_version < "3":
                contents["oneOf"].append(
                    {"$ref": "%s#/definitions/%s" % (prefix, title)}
                )
            else:
                contents["oneOf"].append(
                    {"$ref": (title.replace("#/components/schemas/", "") + ".json")}
                )
        print(
            json_encoder.encode(contents),
            file=all_file,
        )


if __name__ == "__main__":
    default()
