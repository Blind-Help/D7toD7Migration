<?php

/**
 * @file
 * news.inc
 *
 * Migration class for news nodes
 * 
 * This class extends CommonNodeMigration to inherit common field mappings and exclusions
 */

class NewsNodeMigration extends CommonNodeMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Add field mappings
    $this->addFieldMapping(DIGITAL_DETOX_FIELD_NEWS_BODY, DIGITAL_DETOX_FIELD_NEWS_BODY)
      ->sourceMigration(array('paragraph_content', 'paragraph_text', 'paragraph_video'));
    $this->addFieldMapping(DIGITAL_DETOX_FIELD_NEWS_ID, DIGITAL_DETOX_FIELD_NOTIFICATION_ID);
  }
}
