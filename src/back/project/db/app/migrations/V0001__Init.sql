CREATE SCHEMA IF NOT EXISTS project;

CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

CREATE TYPE project.sync_direction AS ENUM ('projectToNode', 'nodeToProject');

CREATE TABLE project.project (
	id UUID PRIMARY KEY DEFAULT UUID_GENERATE_V4(),
	space_id UUID NOT NULL,
	key VARCHAR(32) NOT NULL,
	name VARCHAR(64) NOT NULL,
	description TEXT NOT NULL DEFAULT '',
	changed_at TIMESTAMP NOT NULL DEFAULT NOW(),
	sync project.sync_direction NOT NULL
);

COMMENT ON TABLE project.project IS 'A typical automation object';
COMMENT ON COLUMN project.project.id IS 'Primary key of project';
COMMENT ON COLUMN project.project.space_id IS 'The space that the project belongs to';
COMMENT ON COLUMN project.project.key IS 'Key of project to address it in web interface url and for clients connecting';
COMMENT ON COLUMN project.project.name IS 'Name of the project for appearing in web interface';
COMMENT ON COLUMN project.project.description IS 'Text description of project for appearing in web interface at details tab';
COMMENT ON COLUMN project.project.changed_at IS 'Timestamp of the last change of the project';
COMMENT ON COLUMN project.project.sync IS 'Direction of synchronization with node.';

CREATE TYPE project.param_value_type AS ENUM ('int', 'bool', 'float', 'string', 'bytes', 'time', 'range', 'combo');

COMMENT ON TYPE project.param_value_type IS 'Type of the param value.';

CREATE TABLE project.param_type (
	id BIGSERIAL PRIMARY KEY,
	parent_id BIGINT DEFAULT NULL REFERENCES project.param_type (id) ON DELETE CASCADE,
	key VARCHAR(64) NOT NULL,
	name VARCHAR(64) DEFAULT NULL,
	description VARCHAR(512) NOT NULL,
	value_type project.param_value_type NOT NULL,
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE
);

COMMENT ON TABLE project.param_type IS 'Param types.';
COMMENT ON COLUMN project.param_type.id IS 'Primary key of param type.';
COMMENT ON COLUMN project.param_type.parent_id IS 'Parent param type.';
COMMENT ON COLUMN project.param_type.key IS 'Param type key for addressing from generated API.';
COMMENT ON COLUMN project.param_type.name IS 'Param type name for appearing in web interface.';
COMMENT ON COLUMN project.param_type.description IS 'Text description of param type for appearing in web interface.';
COMMENT ON COLUMN project.param_type.value_type IS 'Type of the param type value.';
COMMENT ON COLUMN project.param_type.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.project_param (
	project_id UUID NOT NULL REFERENCES project.project (id) ON DELETE CASCADE,
	param_id BIGINT NOT NULL REFERENCES project.param_type (id),
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE,
	PRIMARY KEY(project_id, param_id)
);

COMMENT ON TABLE project.project_param IS 'Param types of the project.';
COMMENT ON COLUMN project.project_param.project_id IS 'Project which param type belongs to.';
COMMENT ON COLUMN project.project_param.param_id IS 'Param type.';
COMMENT ON COLUMN project.project_param.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.section (
	id BIGSERIAL PRIMARY KEY,
	project_id UUID NOT NULL REFERENCES project.project (id) ON DELETE CASCADE,
	name VARCHAR(64) DEFAULT NULL,
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE
);

COMMENT ON TABLE project.section IS 'Some kind of room on the automation object. Electrical panel, vestibule and so on.';
COMMENT ON COLUMN project.section.id IS 'Primary key of section.';
COMMENT ON COLUMN project.section.project_id IS 'The project that section belongs to.';
COMMENT ON COLUMN project.section.name IS 'Name for appearing in web interface';
COMMENT ON COLUMN project.section.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.section_param (
	section_id BIGINT NOT NULL REFERENCES project.section (id) ON DELETE CASCADE,
	param_id BIGINT NOT NULL REFERENCES project.param_type (id),
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE,
	PRIMARY KEY(section_id, param_id)
);

COMMENT ON TABLE project.section_param IS 'Param types of sections.';
COMMENT ON COLUMN project.section_param.section_id IS 'Section which param type belongs to.';
COMMENT ON COLUMN project.section_param.param_id IS 'Param type.';
COMMENT ON COLUMN project.section_param.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.cc_type (
	id BIGSERIAL PRIMARY KEY,
	project_id UUID NOT NULL REFERENCES project.project (id) ON DELETE CASCADE,
	key VARCHAR(64) NOT NULL,
	name VARCHAR(64) DEFAULT NULL,
	description TEXT NOT NULL,
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE
);

COMMENT ON TABLE project.cc_type IS 'Control circuit (CC) type - a global and basic control entity that describes some kind of functionality - for example, temperature control, humidity control. It includes a set of some fields, elements, parameters, has a state and a mode.';
COMMENT ON COLUMN project.cc_type.id IS 'Primary key of CC type';
COMMENT ON COLUMN project.cc_type.project_id IS 'The project that CC type belongs to.';
COMMENT ON COLUMN project.cc_type.key IS 'The key of CC type that uses in addressing in automation scripts.';
COMMENT ON COLUMN project.cc_type.name IS 'The name of CC type that appears in web interface.';
COMMENT ON COLUMN project.cc_type.description IS 'The text description of CC type for appearing in web interface.';
COMMENT ON COLUMN project.cc_type.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.control_circuit (
	id BIGSERIAL PRIMARY KEY,
	type_id BIGINT NOT NULL REFERENCES project.cc_type (id),
	section_id BIGINT NOT NULL REFERENCES project.section (id) ON DELETE CASCADE,
	name VARCHAR(64) DEFAULT NULL,
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE
);

COMMENT ON TABLE project.control_circuit IS 'An instance of a control circuit (CC) - for example: Lighting system No. 1, heating system etc';
COMMENT ON COLUMN project.control_circuit.id IS 'Primary key of CC.';
COMMENT ON COLUMN project.control_circuit.type_id IS 'The id of type of CC.';
COMMENT ON COLUMN project.control_circuit.section_id IS 'The section that CC belongs to (in which section which CC exists).';
COMMENT ON COLUMN project.control_circuit.name IS 'The name of the CC that appears in web interface instead of cc_type`s name.';
COMMENT ON COLUMN project.control_circuit.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.plugin (
	id BIGSERIAL PRIMARY KEY,
	project_id UUID NOT NULL REFERENCES project.project (id) ON DELETE CASCADE,
	name VARCHAR(64) DEFAULT NULL,
	description TEXT NOT NULL,
	key VARCHAR(128) NOT NULL,
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE
);

COMMENT ON TABLE project.plugin IS 'A software module describing the order of physical interaction with similar devices (programming environment driver). Describes the information used in the driver of a device.';
COMMENT ON COLUMN project.plugin.id IS 'Primary key of plugin.';
COMMENT ON COLUMN project.plugin.project_id IS 'Project id which plugin belongs to.';
COMMENT ON COLUMN project.plugin.name IS 'The name of the plugin for appearing in web interface.';
COMMENT ON COLUMN project.plugin.description IS 'Text description of plugin for appearing in web interface.';
COMMENT ON COLUMN project.plugin.key IS 'The key of the plugin for addressing it.';
COMMENT ON COLUMN project.plugin.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.device (
	id BIGSERIAL PRIMARY KEY,
	project_id UUID NOT NULL REFERENCES project.project (id) ON DELETE CASCADE,
	plugin_id BIGINT DEFAULT NULL REFERENCES project.plugin (id),
	name VARCHAR(64) NOT NULL,
	check_interval_msec INT NOT NULL,
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE
);

COMMENT ON TABLE project.device IS 'An instance of an external device with a specific type of interface. For example: GPIO, UART device, Modbus device.';
COMMENT ON COLUMN project.device.id IS 'Primary key of device.';
COMMENT ON COLUMN project.device.project_id IS 'Project which device belongs to.';
COMMENT ON COLUMN project.device.plugin_id IS 'Plugin that device depends on.';
COMMENT ON COLUMN project.device.name IS 'The name of the device that appears in web interface.';
COMMENT ON COLUMN project.device.check_interval_msec IS 'Device polling interval.';
COMMENT ON COLUMN project.device.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.device_plugin_param (
	device_id BIGINT NOT NULL REFERENCES project.device (id) ON DELETE CASCADE,
	param_id BIGINT NOT NULL REFERENCES project.param_type (id),
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE,
	PRIMARY KEY(device_id, param_id)
);

COMMENT ON TABLE project.device_plugin_param IS 'Param types of device plugin.';
COMMENT ON COLUMN project.device_plugin_param.device_id IS 'Device which param type belongs to.';
COMMENT ON COLUMN project.device_plugin_param.param_id IS 'Param type.';
COMMENT ON COLUMN project.device_plugin_param.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.code (
	id BIGSERIAL PRIMARY KEY,
	project_id UUID NOT NULL REFERENCES project.project (id) ON DELETE CASCADE,
	repository_id UUID NOT NULL,
	commit_hash TEXT NOT NULL
);

COMMENT ON TABLE project.code IS 'Automation code in an external repository.';
COMMENT ON COLUMN project.code.id IS 'Primary key of code for addressing it.';
COMMENT ON COLUMN project.code.project_id IS 'Project which code belongs to.';
COMMENT ON COLUMN project.code.repository_id IS 'Repository in external SVC system';
COMMENT ON COLUMN project.code.commit_hash IS 'Commit in repository.';

CREATE TABLE project.measure (
	id BIGSERIAL PRIMARY KEY,
	project_id UUID NOT NULL REFERENCES project.project (id) ON DELETE CASCADE,
	name VARCHAR(10) NOT NULL,
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE
);

COMMENT ON TABLE project.measure IS 'Measure of something. For example - Celsius, Fahrenheit etc.';
COMMENT ON COLUMN project.measure.id IS 'Primary key of measure for addressing it.';
COMMENT ON COLUMN project.measure.project_id IS 'Project which measure belongs to.';
COMMENT ON COLUMN project.measure.name IS 'Name of measure for appearing in web interface.';
COMMENT ON COLUMN project.measure.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.save_timer (
	id BIGSERIAL PRIMARY KEY,
	project_id UUID NOT NULL REFERENCES project.project (id) ON DELETE CASCADE,
	interval_msec INT NOT NULL
);

COMMENT ON TABLE project.save_timer IS 'Timers for periodically saving values to a log.';
COMMENT ON COLUMN project.save_timer.id IS 'Primary key for timer.';
COMMENT ON COLUMN project.save_timer.project_id IS 'Project which timer belongs to.';
COMMENT ON COLUMN project.save_timer.interval_msec IS 'Interval of timer in milliseconds';

CREATE TABLE project.cc_type_param (
	cc_type_id BIGINT NOT NULL REFERENCES project.cc_type (id) ON DELETE CASCADE,
	param_id BIGINT NOT NULL REFERENCES project.param_type (id),
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE,
	PRIMARY KEY(cc_type_id, param_id)
);

COMMENT ON TABLE project.cc_type_param IS 'Param types of control circuit (CC) types.';
COMMENT ON COLUMN project.cc_type_param.cc_type_id IS 'CC type which param type belongs to.';
COMMENT ON COLUMN project.cc_type_param.param_id IS 'Param type.';
COMMENT ON COLUMN project.cc_type_param.is_deleted IS 'Is entity deleted.';

CREATE TYPE project.save_algorithm AS ENUM ('off', 'immediately', 'byTimer', 'byTimerOrImmediately');

COMMENT ON TYPE project.save_algorithm IS 'Device item types algorithms for storing values.';

CREATE TYPE project.di_mode AS ENUM ('readonlyFlag', 'readwriteFlag', 'readonly', 'readwrite', 'file', 'button', 'videoStream');

COMMENT ON TYPE project.di_mode IS 'Type of value of device item.';

CREATE TABLE project.di_type (
	id BIGSERIAL PRIMARY KEY,
	measure_id BIGINT DEFAULT NULL REFERENCES project.measure (id) ON DELETE CASCADE,
	save_timer_id BIGINT DEFAULT NULL REFERENCES project.save_timer (id),
	key VARCHAR(64) NOT NULL,
	name VARCHAR(64) DEFAULT NULL,
	mode project.di_mode NOT NULL,
	save_algorithm project.save_algorithm NOT NULL,
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE
);

COMMENT ON TABLE project.di_type IS 'Types of device items (DI).';
COMMENT ON COLUMN project.di_type.id IS 'Primary key of DI type.';
COMMENT ON COLUMN project.di_type.measure_id IS 'Measure for value of DI.';
COMMENT ON COLUMN project.di_type.save_timer_id IS 'Save timer for getting DI value.';
COMMENT ON COLUMN project.di_type.key IS 'Key for DI type for addressing it in API and automation scripts.';
COMMENT ON COLUMN project.di_type.name IS 'Name for DI type for appearing in web interface.';
COMMENT ON COLUMN project.di_type.mode IS 'Type of value of DI type.';
COMMENT ON COLUMN project.di_type.save_algorithm IS 'Save algorithm for storing DI value.';
COMMENT ON COLUMN project.di_type.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.di_plugin_param (
	di_type_id BIGINT NOT NULL REFERENCES project.di_type (id) ON DELETE CASCADE,
	param_id BIGINT NOT NULL REFERENCES project.param_type (id),
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE,
	PRIMARY KEY(di_type_id, param_id)
);

COMMENT ON TABLE project.di_plugin_param IS 'Param types for device item (DI) types.';
COMMENT ON COLUMN project.di_plugin_param.di_type_id IS 'DI type which param type belongs to.';
COMMENT ON COLUMN project.di_plugin_param.param_id IS 'Param type.';
COMMENT ON COLUMN project.di_plugin_param.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.cc_type_di_type (
	cc_type_id BIGINT NOT NULL REFERENCES project.cc_type (id) ON DELETE CASCADE,
	di_type_id BIGINT NOT NULL REFERENCES project.di_type (id),
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE,
	PRIMARY KEY(cc_type_id, di_type_id)
);

COMMENT ON TABLE project.cc_type_di_type IS 'Device item (DI) types for control circuit (CC) types.';
COMMENT ON COLUMN project.cc_type_di_type.cc_type_id IS 'CC type which DI type belongs to.';
COMMENT ON COLUMN project.cc_type_di_type.di_type_id IS 'DI type.';
COMMENT ON COLUMN project.cc_type_di_type.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.device_item (
	id BIGSERIAL PRIMARY KEY,
	device_id BIGINT NOT NULL REFERENCES project.device (id) ON DELETE CASCADE,
	type_id BIGINT NOT NULL REFERENCES project.di_type (id),
	name VARCHAR(64) NOT NULL,
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE
);

COMMENT ON TABLE project.device_item IS 'Device item (DI) - a device component that provides the value/data from a particular sensor or module.';
COMMENT ON COLUMN project.device_item.id IS 'Primary key of DI.';
COMMENT ON COLUMN project.device_item.device_id IS 'Device which DI belongs to.';
COMMENT ON COLUMN project.device_item.type_id IS 'Type of DI.';
COMMENT ON COLUMN project.device_item.name IS 'Name of DI';
COMMENT ON COLUMN project.device_item.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.cc_mode_type (
	id BIGSERIAL PRIMARY KEY,
	cc_type_id BIGINT DEFAULT NULL REFERENCES project.cc_type (id) ON DELETE CASCADE,
	key VARCHAR(64) NOT NULL,
	name VARCHAR(64) NOT NULL,
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE
);

COMMENT ON TABLE project.cc_mode_type IS 'The operating mode of the control circuit (CC) type. For example, manual or automatic.';
COMMENT ON COLUMN project.cc_mode_type.id IS 'Primary key of mode';
COMMENT ON COLUMN project.cc_mode_type.cc_type_id IS 'CC type which mode belongs to. If NULL then valid for all CC types.';
COMMENT ON COLUMN project.cc_mode_type.key IS 'Key of mode for addressing.';
COMMENT ON COLUMN project.cc_mode_type.name IS 'Name of mode that appears in web interface.';
COMMENT ON COLUMN project.cc_mode_type.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.cc_di (
	cc_id BIGINT NOT NULL REFERENCES project.control_circuit (id) ON DELETE CASCADE,
	di_id BIGINT NOT NULL REFERENCES project.device_item (id),
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE,
	PRIMARY KEY(cc_id, di_id)
);

COMMENT ON TABLE project.cc_di IS 'Device items (DI) for control circuit (CC)';
COMMENT ON COLUMN project.cc_di.cc_id IS 'CC which DI belongs to.';
COMMENT ON COLUMN project.cc_di.di_id IS 'DI.';
COMMENT ON COLUMN project.cc_di.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.cc_param (
	cc_id BIGINT NOT NULL REFERENCES project.control_circuit (id) ON DELETE CASCADE,
	param_id BIGINT NOT NULL REFERENCES project.param_type (id),
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE,
	PRIMARY KEY(cc_id, param_id)
);

COMMENT ON TABLE project.cc_param IS 'Param types of control circuit (CC).';
COMMENT ON COLUMN project.cc_param.cc_id IS 'CC which param type belongs to.';
COMMENT ON COLUMN project.cc_param.param_id IS 'Param type.';
COMMENT ON COLUMN project.cc_param.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.cc_status_category (
	id BIGSERIAL PRIMARY KEY,
	project_id UUID NOT NULL REFERENCES project.project (id) ON DELETE CASCADE,
	key VARCHAR(64) NOT NULL,
	name VARCHAR(64) DEFAULT NULL,
	color VARCHAR(16) NOT NULL,
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE
);

COMMENT ON TABLE project.cc_status_category IS 'The status category of the control circuit - for example, warning, error, information, everything is fine.';
COMMENT ON COLUMN project.cc_status_category.id IS 'Primary key of status category.';
COMMENT ON COLUMN project.cc_status_category.project_id IS 'Project which status category belongs to.';
COMMENT ON COLUMN project.cc_status_category.key IS 'Key for addressing status category.';
COMMENT ON COLUMN project.cc_status_category.name IS 'Name of status category for appearing in web interface.';
COMMENT ON COLUMN project.cc_status_category.color IS 'Color of the status category for appearing in web interface.';
COMMENT ON COLUMN project.cc_status_category.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.cc_status_type (
	id BIGSERIAL PRIMARY KEY,
	cc_type_id BIGINT NOT NULL REFERENCES project.cc_type (id) ON DELETE CASCADE,
	category_id BIGINT NOT NULL REFERENCES project.cc_status_category (id),
	key VARCHAR(64) NOT NULL,
	text VARCHAR(512) NOT NULL,
	inform BOOLEAN NOT NULL,
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE
);

COMMENT ON TABLE project.cc_status_type IS 'Type of status of control circuit (CC).';
COMMENT ON COLUMN project.cc_status_type.id IS 'Primary key of type of status.';
COMMENT ON COLUMN project.cc_status_type.cc_type_id IS 'CC type which type of status belongs to.';
COMMENT ON COLUMN project.cc_status_type.category_id IS 'Category of status which type of status belongs to.';
COMMENT ON COLUMN project.cc_status_type.key IS 'Key for addressing type of status in generated API.';
COMMENT ON COLUMN project.cc_status_type.text IS 'Text of type of status for appearing in interface.';
COMMENT ON COLUMN project.cc_status_type.inform IS 'Is it needed to inform user about status type.';
COMMENT ON COLUMN project.cc_status_type.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.value_view (
	id BIGSERIAL PRIMARY KEY,
	di_type_id BIGINT NOT NULL REFERENCES project.di_type (id) ON DELETE CASCADE,
	value TEXT NOT NULL,
	view TEXT NOT NULL,
	is_deleted BOOLEAN NOT NULL DEFAULT FALSE
);

COMMENT ON TABLE project.value_view IS 'Binding of some typical values to some representation. For example, the door has a value of 0 or 1 - closed or open.';
COMMENT ON COLUMN project.value_view.id IS 'Primary key for value and view.';
COMMENT ON COLUMN project.value_view.di_type_id IS 'Device item type which value and view belong to.';
COMMENT ON COLUMN project.value_view.value IS 'Value.';
COMMENT ON COLUMN project.value_view.view IS 'View.';
COMMENT ON COLUMN project.value_view.is_deleted IS 'Is entity deleted.';

CREATE TABLE project.translation (
	id BIGSERIAL PRIMARY KEY,
	project_id UUID NOT NULL REFERENCES project.project (id) ON DELETE CASCADE,
	lang VARCHAR(64) NOT NULL,
	key VARCHAR(64) NOT NULL,
	value TEXT NOT NULL
);

COMMENT ON TABLE project.translation IS 'Translations for interface of the system.';
COMMENT ON COLUMN project.translation.id IS 'Primary key of translation.';
COMMENT ON COLUMN project.translation.project_id IS 'Project which translation belongs to.';
COMMENT ON COLUMN project.translation.lang IS 'Code of language of translation.';
COMMENT ON COLUMN project.translation.key IS 'Key for addressing translation.';
COMMENT ON COLUMN project.translation.value IS 'Value of translation.';