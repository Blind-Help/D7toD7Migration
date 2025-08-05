<?php

/**
 * @file
 * page.inc
 *
 * Migration class for basic page nodes
 * 
 * This class extends CommonNodeMigration to inherit common field mappings and exclusions
 * No special actions are needed here, as all required configurations are handled in the parent class
 */

class PageNodeMigration extends CommonNodeMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);
  }
}
