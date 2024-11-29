/* pgmigrate-encoding: utf-8 */

DO
$do$
BEGIN
	IF EXISTS (
		SELECT FROM pg_catalog.pg_roles
		WHERE rolname = 'svetit_dtlsgateway')
	THEN
		GRANT USAGE ON SCHEMA dtlsgateway TO "svetit_dtlsgateway";
		GRANT ALL ON ALL TABLES IN SCHEMA dtlsgateway TO "svetit_dtlsgateway";
		GRANT ALL ON ALL SEQUENCES IN SCHEMA dtlsgateway TO "svetit_dtlsgateway";
	END IF;
END
$do$;

ALTER USER CURRENT_USER SET TIMEZONE='UTC';
