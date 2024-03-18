/* pgmigrate-encoding: utf-8 */

DO
$do$
BEGIN
	IF EXISTS (
		SELECT FROM pg_catalog.pg_roles
		WHERE rolname = 'svetit_auth')
	THEN
		GRANT USAGE ON SCHEMA auth TO "svetit_auth";
		GRANT ALL ON ALL TABLES IN SCHEMA auth TO "svetit_auth";
		GRANT ALL ON ALL SEQUENCES IN SCHEMA auth TO "svetit_auth";
	END IF;
END
$do$;

ALTER USER CURRENT_USER SET TIMEZONE='UTC';
