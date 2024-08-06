/* pgmigrate-encoding: utf-8 */

DO
$do$
BEGIN
	IF EXISTS (
		SELECT FROM pg_catalog.pg_roles
		WHERE rolname = 'svetit_node')
	THEN
		GRANT USAGE ON SCHEMA node TO "svetit_node";
		GRANT ALL ON ALL TABLES IN SCHEMA node TO "svetit_node";
		GRANT ALL ON ALL SEQUENCES IN SCHEMA node TO "svetit_node";
	END IF;
END
$do$;

ALTER USER CURRENT_USER SET TIMEZONE='UTC';
