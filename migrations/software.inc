<?php

/**
 * @file
 * software.inc
 *
 * Migration class for the Software content type
 * 
 * This class extends CommonNodeMigration to inherit common field mappings and exclusions,
 * while also adding specific mappings for the software node type, such as OS and category fields
 */

class SoftwareNodeMigration extends CommonNodeMigration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->mapSoftwareFields();
    $this->excludeSoftwareFields();
  }

  /**
   * Maps software-specific fields
   */
  protected function mapSoftwareFields() {
    // Map the operating system field to the OS taxonomy from the old site
    $this->mapTaxonomyField(
      DIGITAL_DETOX_FIELD_SOFTWARE_OS,
      DIGITAL_DETOX_FIELD_OPERATING_SYSTEM,
      'operating_systems'
    );

    // Map the software category field to the category taxonomy from the old site
    $this->mapTaxonomyField(
      DIGITAL_DETOX_FIELD_SOFTWARE_CATEGORY,
      DIGITAL_DETOX_FIELD_CATEGORY,
      'categories'
    );

    // Map the download link field and its attributes
    $this->mapLinkField(
      DIGITAL_DETOX_FIELD_SOFTWARE_DL,
      DIGITAL_DETOX_FIELD_DOWNLOAD_LINK
    );
  }

  /**
   * Excludes fields that are not needed for software migration
   */
  protected function excludeSoftwareFields() {
    $this->excludeCommonFields(array(
      DIGITAL_DETOX_FIELD_OPERATING_SYSTEM . ':language',
      DIGITAL_DETOX_FIELD_CATEGORY . ':language',
      DIGITAL_DETOX_FIELD_DOWNLOAD_LINK . ':language'
    ));
  }

  /**
   * Builds the query to migrate software nodes, excluding those tagged as "Game"
   *
   * This query excludes nodes with the term ID for "Game" in the category field,
   * ensuring that only non-game software items are included in the migration
   *
   * @return \SelectQueryExtender
   *   The modified query object for the migration
   */
  protected function query() {
    // Retrieve the default query from the parent migration class
    $query = parent::query();

    // Subquery to select node IDs associated with the "Game" term
    // This subquery will help exclude nodes tagged with "Game"
    $subquery = db_select('taxonomy_index', 'ti')
      ->fields('ti', array('nid'))
      ->condition('ti.tid', DIGITAL_DETOX_GAME_TERM_ID);

    // Exclude nodes from the main query where nid is in the subquery result
    // This ensures no nodes tagged with "Game" are included in the software migration
    $query->condition('n.nid', $subquery, 'NOT IN');

    return $query;
  }
}
