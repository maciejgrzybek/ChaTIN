CREATE TABLE `group` ( `id` INTEGER NOT NULL, `name` TEXT NOT NULL, PRIMARY KEY(`id`), UNIQUE(`name`) );
CREATE TABLE `alias` ( `ip` TEXT NOT NULL, `alias` TEXT NOT NULL, `groupId` INTEGER NOT NULL, PRIMARY KEY(`ip`), UNIQUE(`alias`), FOREIGN KEY(`groupId`) REFERENCES `group`(id) );
CREATE TABLE `message` ( `id` INTEGER NOT NULL, `content` TEXT NOT NULL, `date` INTEGER NOT NULL, `ip` TEXT, `isOutgoing` INTEGER NOT NULL, `conferenceId` INTEGER,  PRIMARY KEY(`id`), FOREIGN KEY(`conferenceId`) REFERENCES `conference`(`id`) );
CREATE TABLE `subscription` ( `ip` TEXT NOT NULL, `state` INTEGER NOT NULL, `date` INTEGER NOT NULL, PRIMARY KEY(`ip`) );
CREATE TABLE `conference` ( `id` INTEGER NOT NULL, `ownerIp` TEXT NOT NULL, `name` TEXT NOT NULL, `date` INTEGER NOT NULL, PRIMARY KEY(`id`) );
CREATE TABLE `conferenceMember` ( `id` INTEGER NOT NULL, `ip` TEXT NOT NULL, `conferenceId` INTEGER NOT NULL, PRIMARY KEY(`id`), FOREIGN KEY(`conferenceId`) REFERENCES `conference`(`id`) );

CREATE TRIGGER alias_unique BEFORE INSERT ON alias
BEGIN
SELECT CASE
WHEN
(
    SELECT COUNT() as cnt FROM
        (SELECT alias from alias WHERE alias = new.alias)
        WHERE cnt > 0
)
THEN RAISE(FAIL,'Alias already exists')
END;
END;

CREATE INDEX alias_alias_idx ON alias(alias);
CREATE INDEX alias_ip_idx ON alias(ip);
CREATE INDEX alias_group_idx ON alias(groupId);
CREATE INDEX message_ip_idx ON message(ip);
CREATE INDEX message_conference_idx ON message(conferenceId);
CREATE INDEX subscription_idx ON subscription(ip);
CREATE INDEX group_idx ON `group`(id);
CREATE INDEX conference_idx ON conference(ownerIp,name);
CREATE INDEX conferenceMember_idx ON conferenceMember(conferenceId);
