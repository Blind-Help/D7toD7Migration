<?php

/**
 * @file
 * digital_detox.node.inc
 *
 * Migration class for nodes
 * 
 * This migration handles the migration of node data, including titles, 
 * body content, and additional common fields. It migrates only published 
 * content and excludes unnecessary fields like revision information and 
 * language settings
 */

abstract class CommonNodeMigration extends DrupalNode7Migration {
  use DigitalDetoxMigrationUtilities;

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->mapEssentialFields();
    $this->mapAuthorField();
    $this->excludeUnnecessaryFields();
  }

  /**
   * Maps essential node fields
   */
  protected function mapEssentialFields() {
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('body:summary', 'body:summary');
    $this->addFieldMapping('body:format')
      ->defaultValue(DIGITAL_DETOX_DEFAULT_TEXT_FORMAT);
    $this->addFieldMapping('is_new')
      ->defaultValue(FALSE);
  }

  /**
   * Maps the author field with proper fallback
   */
  protected function mapAuthorField() {
    $this->addFieldMapping('uid', 'uid')
      ->sourceMigration('user')
      ->defaultValue(DIGITAL_DETOX_DEFAULT_USER_ID);
  }

  /**
   * Excludes unnecessary fields from the destination
   */
  protected function excludeUnnecessaryFields() {
    $this->addUnmigratedDestinations(array(
      'body:language',
      'nid',
      'promote',
      'sticky',
      'revision',
      'log',
      'language',
      'tnid',
      'revision_uid',
      'vid',
      'comment',
      'translate'
    ));
  }

  /**
   * Prevents stub creation during migration
   *
   * @param object $migration
   *   The migration object
   *
   * @return bool
   *   Always returns FALSE to prevent stub creation
   */
  protected function createStub($migration) {
    return FALSE;
  }

  /**
   * Override the query method to filter published content only
   */
  protected function query() {
    // Get the default query from DrupalNode7Migration
    $query = parent::query();

    // Filter to only include published nodes
    $query->condition('n.status', 1);

    return $query;
  }

  /**
   * Validates row data before processing to prevent migration failures
   */
  public function prepareRow($row) {
    if (!$this->validateNodeRow($row)) {
      return FALSE;
    }

    return parent::prepareRow($row);
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
}
