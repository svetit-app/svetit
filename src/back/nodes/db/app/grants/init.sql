/* pgmigrate-encoding: utf-8 */

DO
$do$
BEGIN
	IF EXISTS (
		SELECT FROM pg_catalog.pg_roles
		WHERE rolname = 'svetit_nodes')
	THEN
		GRANT USAGE ON SCHEMA nodes TO "svetit_nodes";
		GRANT ALL ON ALL TABLES IN SCHEMA nodes TO "svetit_nodes";
		GRANT ALL ON ALL SEQUENCES IN SCHEMA nodes TO "svetit_nodes";
	END IF;
END
$do$;

ALTER USER CURRENT_USER SET TIMEZONE='UTC';
