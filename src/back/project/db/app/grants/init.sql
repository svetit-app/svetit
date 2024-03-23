/* pgmigrate-encoding: utf-8 */

DO
$do$
BEGIN
	IF EXISTS (
		SELECT FROM pg_catalog.pg_roles
		WHERE rolname = 'svetit_project')
	THEN
		GRANT USAGE ON SCHEMA project TO "svetit_project";
		GRANT ALL ON ALL TABLES IN SCHEMA project TO "svetit_project";
		GRANT ALL ON ALL SEQUENCES IN SCHEMA project TO "svetit_project";
	END IF;
END
$do$;

ALTER USER CURRENT_USER SET TIMEZONE='UTC';
