<?php

/**
 * @file
 * digital_detox.paragraphs.inc
 *
 * Utilities specific to paragraph migrations
 */

/**
 * Utility trait for paragraph-specific functionality
 */
trait DigitalDetoxParagraphUtilities {

  /**
   * Validates paragraph item data
   *
   * @param object $row
   *   The row object containing paragraph data
   *
   * @return bool
   *   TRUE if valid, FALSE otherwise
   */
  protected function validateParagraphRow($row) {
    if (empty($row->item_id) || !is_numeric($row->item_id)) {
      $this->logMigrationError('Invalid paragraph item_id: @item_id', array('@item_id' => $row->item_id));
      return FALSE;
    }

    if (empty($row->bundle)) {
      $this->logMigrationError('Missing paragraph bundle for item_id: @item_id', array('@item_id' => $row->item_id));
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Fetches content references for a paragraph item
   *
   * @param int $item_id
   *   The paragraph item ID
   *
   * @return array
   *   Array of target node IDs
   */
  protected function fetchContentReferences($item_id) {
    $query = Database::getConnection('default', DIGITAL_DETOX_SOURCE_CONNECTION)
      ->select('field_data_field_content', 'fc')
      ->fields('fc', array('field_content_target_id'))
      ->condition('entity_id', $item_id)
      ->condition('entity_type', 'paragraphs_item')
      ->condition('field_content_target_id', 0, '>')
      ->orderBy('delta');

    $results = $query->execute();
    $target_ids = array();

    foreach ($results as $result) {
      if ($this->validateId($result->field_content_target_id)) {
        $target_ids[] = $result->field_content_target_id;
      }
    }

    return $target_ids;
  }

  /**
   * Handles content reference migration with error handling
   *
   * @param array $target_ids
   *   Array of target node IDs
   * @param array $source_migrations
   *   Array of source migration names to try
   *
   * @return array
   *   Array of successfully migrated node IDs
   */
  protected function handleContentReferences(array $target_ids, array $source_migrations) {
    $migrated_ids = array();

    foreach ($target_ids as $target_id) {
      $nid = $this->handleSourceMigration($source_migrations, $target_id);
      if ($nid && $this->validateId($nid)) {
        $migrated_ids[] = $nid;
      }
    }

    return $migrated_ids;
  }

  /**
   * Creates paragraph field array structure
   *
   * @param array $target_ids
   *   Array of target node IDs
   *
   * @return array
   *   Properly structured field array
   */
  protected function createParagraphFieldArray(array $target_ids) {
    $field_array = array();
    
    foreach ($target_ids as $target_id) {
      $field_array[LANGUAGE_NONE][] = array('target_id' => $target_id);
    }

    return $field_array;
  }

  /**
   * Validates paragraph field data
   *
   * @param mixed $value
   *   The field value to validate
   *
   * @return bool
   *   TRUE if valid, FALSE otherwise
   */
  protected function validateParagraphField($value) {
    return !is_null($value) && $value !== '';
  }

  /**
   * Validates YouTube video link format
   *
   * @param string $link
   *   The video link to validate
   *
   * @return bool
   *   TRUE if valid YouTube link, FALSE otherwise
   */
  protected function isValidYouTubeLink($link) {
    if (empty($link)) {
      return FALSE;
    }

    $youtube_patterns = array(
      '/^https?:\/\/(www\.)?youtube\.com\/watch\?v=[a-zA-Z0-9_-]+/',
      '/^https?:\/\/youtu\.be\/[a-zA-Z0-9_-]+/'
    );

    foreach ($youtube_patterns as $pattern) {
      if (preg_match($pattern, $link)) {
        return TRUE;
      }
    }
    
    return FALSE;
  }
}
