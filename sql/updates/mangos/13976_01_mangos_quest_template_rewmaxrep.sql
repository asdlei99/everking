ALTER TABLE db_version CHANGE COLUMN required_13975_01_mangos_creature_waypoint_refurbishing required_13976_01_mangos_quest_template_rewmaxrep bit;

ALTER TABLE quest_template
ADD COLUMN RewMaxRepValue1 MEDIUMINT(8) NOT NULL DEFAULT 42999 AFTER RewRepValue5,
ADD COLUMN RewMaxRepValue2 MEDIUMINT(8) NOT NULL DEFAULT 42999 AFTER RewMaxRepValue1,
ADD COLUMN RewMaxRepValue3 MEDIUMINT(8) NOT NULL DEFAULT 42999 AFTER RewMaxRepValue2,
ADD COLUMN RewMaxRepValue4 MEDIUMINT(8) NOT NULL DEFAULT 42999 AFTER RewMaxRepValue3,
ADD COLUMN RewMaxRepValue5 MEDIUMINT(8) NOT NULL DEFAULT 42999 AFTER RewMaxRepValue4;

