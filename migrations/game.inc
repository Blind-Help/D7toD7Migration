<?php

/**
 * @file
 * game.inc
 *
 * Migration class for the Game content type
 * 
 * This class extends CommonNodeMigration to inherit common field mappings and exclusions,
 * while also adding specific mappings for the game node type, such as platform and genre fields
 */

class GameNodeMigration extends CommonNodeMigration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->mapGameFields();
    $this->excludeGameFields();
  }

  /**
   * Maps game-specific fields
   */
  protected function mapGameFields() {
    // Map the platform field to the OS taxonomy from the old site
    $this->mapTaxonomyField(
      DIGITAL_DETOX_FIELD_SOFTWARE_OS,
      DIGITAL_DETOX_FIELD_OPERATING_SYSTEM,
      'operating_systems'
    );

    // Map the download link field and its attributes
    $this->mapLinkField(
      DIGITAL_DETOX_FIELD_SOFTWARE_DL,
      DIGITAL_DETOX_FIELD_DOWNLOAD_LINK
    );
  }

  /**
   * Excludes fields that are not needed for game migration
   */
  protected function excludeGameFields() {
    $this->excludeCommonFields(array(
      DIGITAL_DETOX_FIELD_OPERATING_SYSTEM . ':language',
      DIGITAL_DETOX_FIELD_DOWNLOAD_LINK . ':language',
      'path'
    ));
  }

  /**
   * Builds the query to migrate only game nodes
   *
   * This query includes only nodes with the term ID for "Game" in the category field,
   * filtering to ensure that only game items are included in the migration
   *
   * @return \SelectQueryExtender
   *   The modified query object for the migration
   */
  protected function query() {
    // Retrieve the default query from the parent migration class
    $query = parent::query();

    // Join the taxonomy_index table to filter nodes by term ID
    // This allows us to include only nodes tagged with the "Game" term
    $query->innerJoin('taxonomy_index', 'ti', 'ti.nid = n.nid');

    // Include only nodes where the term ID matches "Game"
    $query->condition('ti.tid', DIGITAL_DETOX_GAME_TERM_ID);

    return $query;
  }
}
