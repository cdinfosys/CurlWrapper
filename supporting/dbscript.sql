CREATE DATABASE IF NOT EXISTS  puxuluj_DemoDB;

DROP TABLE IF EXISTS puxuluj_DemoDB.CurlWrapperTesting;
CREATE TABLE puxuluj_DemoDB.CurlWrapperTesting
(
	CurlWrapperTestingId int unsigned NOT NULL PRIMARY KEY,
    DataValue int NOT NULL,
    ErrorMessage varchar(128) NULL
);
