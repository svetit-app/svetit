kubernetes_definitions = {
    # Although the kubernetes api does not allow `number` as valid
    # Quantity type - almost all kubenetes tooling
    # recognizes it is valid. For this reason, we extend the API definition to
    # allow `number` values.
    "io.k8s.apimachinery.pkg.util.intstr.IntOrString": {
        "oneOf": [{"type": "string"}, {"type": "integer"}]
    },
    "io.k8s.apimachinery.pkg.api.resource.Quantity": {
        "oneOf": [{"type": "string"}, {"type": "number"}]
    },
    # Replace `JSONSchemaProps` with a dummy object type to prevent circular
    # references
    "io.k8s.apiextensions-apiserver.pkg.apis.apiextensions.v1.JSONSchemaProps": {
        "type": "object",
    },
}
