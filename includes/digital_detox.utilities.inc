<?php

/**
 * @file
 * digital_detox.utilities.inc
 *
 * Utility functions and traits for the Digital Detox migration module
 */

/**
 * Utility trait for common migration functionality
 */
trait DigitalDetoxMigrationUtilities {

  /**
   * Ensures attributes are returned as an array, unserializing if necessary
   *
   * Processes the attributes to confirm array format, unserializing strings when needed
   *
   * @param mixed $attributes
   *   A serialized string, array, or single value
   *
   * @return array
   *   Processed attributes as an array
   */
  protected function transformAttributes($attributes) {
    // If attributes is an array, unserialize any string entries
    if (is_array($attributes)) {
      foreach ($attributes as &$attribute) {
        if (is_string($attribute)) {
          $attribute = unserialize($attribute);
        }
      }
      return $attributes;
    }

    // If attributes is a single string, unserialize it
    if (is_string($attributes)) {
      $attributes = unserialize($attributes);
    }

    // Return an empty array if unserialized result is not an array
    return is_array($attributes) ? $attributes : array();
  }

  /**
   * Formats taxonomy term names consistently
   *
   * @param string $name
   *   The original term name from the source
   *
   * @return string
   *   The formatted term name
   */
  protected function formatTermName($name) {
    // Trim whitespace from the term name
    $formatted_name = trim($name);

    // Handle terms with parentheses like "English (American)"
    if (strpos($formatted_name, '(') !== false && strpos($formatted_name, ')') !== false) {
      // Split by parentheses and format each part
      $parts = preg_split('/[()]/', $formatted_name, -1, PREG_SPLIT_NO_EMPTY);
      $formatted_parts = array();
      
      foreach ($parts as $part) {
        $part = trim($part);
        if (!empty($part)) {
          // Remove special characters except parentheses, allowing letters, numbers, spaces, and & symbol
          $part = preg_replace('/[^a-zA-Z0-9\s&()]/', '', $part);
          $part = preg_replace('/\s+/', ' ', $part);
          $formatted_parts[] = ucwords(strtolower($part));
        }
      }
      
      // Reconstruct with parentheses
      $formatted_name = $formatted_parts[0];
      if (isset($formatted_parts[1])) {
        $formatted_name .= ' (' . $formatted_parts[1] . ')';
      }
    } else {
      // Standard formatting for terms without parentheses
      // Remove special characters, allowing only letters, numbers, spaces, and the & symbol
      $formatted_name = preg_replace('/[^a-zA-Z0-9\s&]/', '', $formatted_name);

      // Replace multiple spaces with a single space
      $formatted_name = preg_replace('/\s+/', ' ', $formatted_name);

      // Convert the term name to title case (capitalize first letter of each word)
      $formatted_name = ucwords(strtolower($formatted_name));
    }

    return $formatted_name;
  }

  /**
   * Validates and sanitizes an ID
   *
   * @param mixed $id
   *   The ID to validate
   *
   * @return int|false
   *   The validated ID or FALSE if invalid
   */
  protected function validateId($id) {
    if (is_numeric($id) && $id > 0) {
      return (int) $id;
    }
    return FALSE;
  }

  /**
   * Logs migration errors consistently
   *
   * @param string $message
   *   The error message to log
   * @param array $context
   *   Additional context information
   */
  protected function logMigrationError($message, array $context = array()) {
    watchdog('digital_detox', $message, $context, WATCHDOG_ERROR);
  }

  /**
   * Excludes common fields to prevent migration failures
   *
   * @param array $additional_fields
   *   Additional fields to exclude beyond the common ones
   */
  protected function excludeCommonFields(array $additional_fields = array()) {
    $excluded_fields = array_merge(
      DIGITAL_DETOX_EXCLUDED_IMAGE_FIELDS,
      DIGITAL_DETOX_EXCLUDED_COMMON_FIELDS,
      $additional_fields
    );
    
    $this->addUnmigratedSources($excluded_fields);
  }

  /**
   * Maps taxonomy fields with consistent configuration
   *
   * @param string $destination_field
   *   The destination field name
   * @param string $source_field
   *   The source field name
   * @param string $source_migration
   *   The source migration name
   */
  protected function mapTaxonomyField($destination_field, $source_field, $source_migration) {
    $this->addFieldMapping($destination_field, $source_field)
      ->sourceMigration($source_migration);
    $this->addFieldMapping($destination_field . ':source_type')
      ->defaultValue('tid');
    $this->addFieldMapping($destination_field . ':ignore_case')
      ->defaultValue(TRUE);
  }

  /**
   * Maps link fields with attributes handling
   *
   * @param string $destination_field
   *   The destination field name
   * @param string $source_field
   *   The source field name
   */
  protected function mapLinkField($destination_field, $source_field) {
    $this->addFieldMapping($destination_field, $source_field);
    $this->addFieldMapping($destination_field . ':title', $source_field . ':title');
    $this->addFieldMapping($destination_field . ':attributes', $source_field . ':attributes')
      ->callbacks(array($this, 'transformAttributes'));
  }

  /**
   * Validates essential node data to prevent migration failures
   *
   * @param object $row
   *   The row object containing node data
   *
   * @return bool
   *   TRUE if valid, FALSE otherwise
   */
  protected function validateNodeRow($row) {
    // Validate node ID
    if (!$this->validateId($row->nid)) {
      $this->logMigrationError('Invalid node ID: @nid', array('@nid' => $row->nid));
      return FALSE;
    }

    // Validate title exists and is not empty
    if (empty($row->title) || !is_string($row->title)) {
      $this->logMigrationError('Missing or invalid title for node @nid', array('@nid' => $row->nid));
      return FALSE;
    }

    // Validate node is published
    if (!isset($row->status) || $row->status != 1) {
      $this->logMigrationError('Node @nid is not published, skipping', array('@nid' => $row->nid));
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Validates taxonomy term data to prevent migration failures
   *
   * @param object $row
   *   The row object containing term data
   *
   * @return bool
   *   TRUE if valid, FALSE otherwise
   */
  protected function validateTermRow($row) {
    // Validate term ID
    if (!$this->validateId($row->tid)) {
      $this->logMigrationError('Invalid term ID: @tid', array('@tid' => $row->tid));
      return FALSE;
    }

    // Validate term name exists and is not empty
    if (empty($row->name) || !is_string($row->name)) {
      $this->logMigrationError('Missing or invalid name for term @tid', array('@tid' => $row->tid));
      return FALSE;
    }

    // Skip the game term as it's handled separately
    if ($row->tid == DIGITAL_DETOX_GAME_TERM_ID) {
      return FALSE;
    }

    return TRUE;
  }
}
