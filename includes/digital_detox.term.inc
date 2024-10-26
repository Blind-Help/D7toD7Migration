<?php

/**
 * @file
 * digital_detox.term.inc
 *
 * Migration class for taxonomy terms.
 * 
 * This migration handles the migration of taxonomy term data. It migrates 
 * only the term name, applying specific formatting: trimming whitespace, 
 * removing special characters, and converting the term name to title case. 
 * Unnecessary fields are excluded from the migration.
 */

class TermMigration extends DrupalTerm7Migration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Mapping and formatting the term name.
    $this->addFieldMapping('name', 'name')                // Term name.
      ->callbacks(array($this, 'formatTermName'));        // Apply formatting via callback.

    // Excluding Fields from Destination.
    $this->addUnmigratedDestinations(array(
      'tid',             // Term ID (auto-assigned on destination).
      'description',     // Skip term description.
      'weight',          // Skip term weight.
      'format',          // Skip text format settings.
    ));
  }

  /**
   * Format term names before migration.
   *
   * - Trims whitespace.
   * - Removes unwanted special characters, but allows specific ones like &.
   * - Allows only a single space between words.
   * - Converts the name to title case (first letter of each word capitalized).
   *
   * @param string $name
   *   The original term name from the source.
   * 
   * @return string
   *   The formatted term name.
   */
  public function formatTermName($name) {
    // Trim whitespace from the term name.
    $formatted_name = trim($name);

    // Remove special characters, allowing only letters, numbers, spaces, and the & symbol.
    $formatted_name = preg_replace('/[^a-zA-Z0-9\s&]/', '', $formatted_name);

    // Replace multiple spaces with a single space.
    $formatted_name = preg_replace('/\s+/', ' ', $formatted_name);

    // Convert the term name to title case (capitalize first letter of each word).
    $formatted_name = ucwords(strtolower($formatted_name));

    return $formatted_name;
  }
}
