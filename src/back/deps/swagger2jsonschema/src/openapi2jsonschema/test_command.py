import filecmp
import os

import pytest  # type: ignore
from click.testing import CliRunner

from .command import default

FIXTURE_DIR = os.path.realpath(
    os.path.join(os.path.dirname(__file__), "..", "fixtures")
)


def test_help():
    runner = CliRunner()
    result = runner.invoke(default, ["--help"])
    assert result.exit_code == 0
    assert "Usage: default [OPTIONS] SCHEMA_URL" in result.output


@pytest.mark.datafiles(
    os.path.join(FIXTURE_DIR, "oasv3", "petstore.yaml"),
    os.path.join(FIXTURE_DIR, "oasv3", "petstore.json"),
)
def test_cmd_oasv3(datafiles):
    runner = CliRunner()
    for spec in os.listdir(datafiles):
        output_dir = os.path.join(datafiles, "schemas")
        schema_url = os.path.join(datafiles, spec)
        result = runner.invoke(
            default,
            [
                "--output",
                output_dir,
                schema_url,
            ],
        )
        assert result.exit_code == 0
        assert filecmp.cmp(
            os.path.join(FIXTURE_DIR, "oasv3", "schemas", "all.json"),
            os.path.join(output_dir, "all.json"),
        )
        assert filecmp.cmp(
            os.path.join(FIXTURE_DIR, "oasv3", "schemas", "error.json"),
            os.path.join(output_dir, "error.json"),
        )
        assert filecmp.cmp(
            os.path.join(FIXTURE_DIR, "oasv3", "schemas", "pet.json"),
            os.path.join(output_dir, "pet.json"),
        )
        assert filecmp.cmp(
            os.path.join(FIXTURE_DIR, "oasv3", "schemas", "pets.json"),
            os.path.join(output_dir, "pets.json"),
        )


@pytest.mark.datafiles(
    os.path.join(FIXTURE_DIR, "kubernetes", "swagger.json"),
)
def test_cmd_k8s_local(datafiles):
    runner = CliRunner()
    for spec in os.listdir(datafiles):
        output_dir = os.path.join(datafiles, "schemas")
        schema_url = os.path.join(datafiles, spec)
        result = runner.invoke(
            default,
            [
                "--output",
                output_dir,
                "--kubernetes",
                "--expanded",
                schema_url,
            ],
        )
        assert result.exit_code == 0
        for schema in [
            "_definitions.json",
            "all.json",
            "pod-v1.json",
            "daemonset-apps-v1.json",
            "helmchart-helm.cattle.io-v1.json",
            "middleware-traefik.io-v1alpha1.json",
            "middleware-traefik.containo.us-v1alpha1.json",
        ]:
            assert filecmp.cmp(
                os.path.join(FIXTURE_DIR, "kubernetes", "schemas", "local", schema),
                os.path.join(output_dir, schema),
            )


@pytest.mark.datafiles(
    os.path.join(FIXTURE_DIR, "kubernetes", "swagger.json"),
)
def test_cmd_k8s_local_strict(datafiles):
    runner = CliRunner()
    for spec in os.listdir(datafiles):
        output_dir = os.path.join(datafiles, "schemas")
        schema_url = os.path.join(datafiles, spec)
        result = runner.invoke(
            default,
            [
                "--output",
                output_dir,
                "--kubernetes",
                "--expanded",
                "--strict",
                schema_url,
            ],
        )
        assert result.exit_code == 0
        for schema in [
            "_definitions.json",
            "all.json",
            "pod-v1.json",
            "daemonset-apps-v1.json",
            "helmchart-helm.cattle.io-v1.json",
            "middleware-traefik.io-v1alpha1.json",
            "middleware-traefik.containo.us-v1alpha1.json",
        ]:
            assert filecmp.cmp(
                os.path.join(
                    FIXTURE_DIR, "kubernetes", "schemas", "local-strict", schema
                ),
                os.path.join(output_dir, schema),
            )


@pytest.mark.datafiles(
    os.path.join(FIXTURE_DIR, "kubernetes", "swagger.json"),
)
def test_cmd_k8s_standalone(datafiles):
    runner = CliRunner()
    for spec in os.listdir(datafiles):
        output_dir = os.path.join(datafiles, "schemas")
        schema_url = os.path.join(datafiles, spec)
        result = runner.invoke(
            default,
            [
                "--output",
                output_dir,
                "--kubernetes",
                "--expanded",
                "--stand-alone",
                schema_url,
            ],
        )
        assert result.exit_code == 0
        for schema in [
            "_definitions.json",
            "all.json",
            "pod-v1.json",
            "daemonset-apps-v1.json",
            "helmchart-helm.cattle.io-v1.json",
            "middleware-traefik.io-v1alpha1.json",
            "middleware-traefik.containo.us-v1alpha1.json",
        ]:
            assert filecmp.cmp(
                os.path.join(
                    FIXTURE_DIR, "kubernetes", "schemas", "standalone", schema
                ),
                os.path.join(output_dir, schema),
            )


@pytest.mark.datafiles(
    os.path.join(FIXTURE_DIR, "kubernetes", "swagger.json"),
)
def test_cmd_k8s_standalone_strict(datafiles):
    runner = CliRunner()
    for spec in os.listdir(datafiles):
        output_dir = os.path.join(datafiles, "schemas")
        schema_url = os.path.join(datafiles, spec)
        result = runner.invoke(
            default,
            [
                "--output",
                output_dir,
                "--kubernetes",
                "--expanded",
                "--stand-alone",
                "--strict",
                schema_url,
            ],
        )
        assert result.exit_code == 0
        for schema in [
            "_definitions.json",
            "all.json",
            "pod-v1.json",
            "daemonset-apps-v1.json",
            "helmchart-helm.cattle.io-v1.json",
            "middleware-traefik.io-v1alpha1.json",
            "middleware-traefik.containo.us-v1alpha1.json",
        ]:
            assert filecmp.cmp(
                os.path.join(
                    FIXTURE_DIR, "kubernetes", "schemas", "standalone-strict", schema
                ),
                os.path.join(output_dir, schema),
            )
