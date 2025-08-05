<?php

/**
 * @file
 * digital_detox.user.inc
 *
 * Migration class for users
 * 
 * This migration handles user data, migrating only essential fields: 
 * username, email, and password. All users are assigned the 'authenticated' 
 * role and set as active by default. Unnecessary fields are excluded from 
 * the migration
 */

class UserMigration extends DrupalUser7Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Mapping Essential Fields
    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('mail', 'mail');
    $this->addFieldMapping('pass', 'pass');
    $this->addFieldMapping('init', 'init');
    $this->addFieldMapping('timezone')
      ->defaultValue('Asia/Karachi');
    $this->addFieldMapping('roles')
      ->defaultValue(DRUPAL_AUTHENTICATED_RID);
    $this->addFieldMapping('status')
      ->defaultValue(1);

    // Excluding Fields from Destination
    $this->addUnmigratedDestinations(array(
      'uid',
      'picture',
      'signature',
      'signature_format',
      'language',
      'theme',
      'path',
      'changed'
    ));

    // Excluding Fields from Source
    $this->addUnmigratedSources(array(
      'data'
    ));
  }

  /**
   * Override addFieldMapping to suppress override warnings
   *
   * @param string $destination_field
   *   The destination field name
   * @param string $source_field
   *   The source field name
   * @param bool $warn_on_override
   *   Whether to warn on override (ignored, always FALSE)
   *
   * @return object
   *   The field mapping object
   */
  public function addFieldMapping($destination_field, $source_field = NULL, $warn_on_override = TRUE) {
    return parent::addFieldMapping($destination_field, $source_field, FALSE);
  }

  /**
   * Override prepareRow to handle init field fallback logic
   *
   * @param object $row
   *   The source row object
   *
   * @return bool
   *   TRUE to process this row, FALSE to skip it
   */
  public function prepareRow($row) {
    // Call parent prepareRow first
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // If init is empty or null, use mail value as fallback
    if (empty($row->init)) {
      $row->init = $row->mail;
    }

    return TRUE;
  }
}
