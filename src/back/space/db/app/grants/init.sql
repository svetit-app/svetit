/* pgmigrate-encoding: utf-8 */

DO
$do$
BEGIN
	IF EXISTS (
		SELECT FROM pg_catalog.pg_roles
		WHERE rolname = 'svetit_space')
	THEN
		GRANT USAGE ON SCHEMA space TO "svetit_space";
		GRANT ALL ON ALL TABLES IN SCHEMA space TO "svetit_space";
		GRANT ALL ON ALL SEQUENCES IN SCHEMA space TO "svetit_space";
	END IF;
END
$do$;

