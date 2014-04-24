CREATE TABLE `CrawlingResult` (
    `title` varchar(128) NOT NULL, 
    `url`   varchar(256) NOT NULL, 
    `date`  TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE UNIQUE INDEX url_idx ON `CrawlingResult`(`url`);
CREATE INDEX date_idx ON `CrawlingResult`(`date`);
