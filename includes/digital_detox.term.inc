<?php

/**
 * @file
 * digital_detox.term.inc
 *
 * Migration class for taxonomy terms
 * 
 * This migration handles the migration of taxonomy term data. It migrates 
 * only the term name, applying specific formatting: trimming whitespace, 
 * removing special characters, and converting the term name to title case 
 * Unnecessary fields are excluded from the migration
 */

class TermMigration extends DrupalTerm7Migration {
  use DigitalDetoxMigrationUtilities;

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->mapTermFields();
    $this->excludeUnnecessaryFields();
  }

  /**
   * Maps term fields with proper formatting
   */
  protected function mapTermFields() {
    // Mapping and formatting the term name
    $this->addFieldMapping('name', 'name')
      ->callbacks(array($this, 'formatTermName'));
  }

  /**
   * Excludes unnecessary fields from the destination
   */
  protected function excludeUnnecessaryFields() {
    $this->addUnmigratedDestinations(array(
      'tid',
      'description',
      'weight',
      'format'
    ));
  }

  /**
   * Builds the query to migrate taxonomy terms, excluding the "Game" term
   *
   * This query excludes the "Game" term from the term migration,
   * as it is now represented as a separate content type
   *
   * @return \SelectQueryExtender
   *   The modified query object for the term migration
   */
  protected function query() {
    // Retrieve the default query from the parent migration class
    $query = parent::query();

    // Exclude the "Game" term by referencing the existing alias `td`
    $query->condition('td.tid', DIGITAL_DETOX_GAME_TERM_ID, '!=');

    return $query;
  }

  /**
   * Validates row data before processing to prevent migration failures
   */
  public function prepareRow($row) {
    if (!$this->validateTermRow($row)) {
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
