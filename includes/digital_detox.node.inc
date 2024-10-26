<?php

/**
 * @file
 * digital_detox.node.inc
 *
 * Migration class for nodes.
 * 
 * This migration handles the migration of node data, including titles, 
 * body content, and additional common fields. It migrates only published 
 * content and excludes unnecessary fields like revision information and 
 * language settings.
 */

abstract class CommonNodeMigration extends DrupalNode7Migration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Mapping Essential Fields.
    $this->addFieldMapping('title', 'title');               // Node title.
    $this->addFieldMapping('body', 'body');                 // Node body content.
    $this->addFieldMapping('body:summary', 'body:summary'); // Body summary.
    $this->addFieldMapping('body:format')                   // Text format for body.
      ->defaultValue('full_html');
    $this->addFieldMapping('is_new')                        // Treat all nodes as not new.
      ->defaultValue(FALSE);

    // Author Mapping.
    $this->addFieldMapping('uid', 'uid')                    // Map node author from user migration.
      ->sourceMigration('user')
      ->defaultValue(1);                                    // Default to user ID 1 if no match.

    // Excluding Fields from Destination.
    $this->addUnmigratedDestinations(array(
      'body:language',    // Exclude language field for the body.
      'nid',              // Node ID (auto-assigned on destination).
      'promote',          // Promoted to front page (not necessary).
      'sticky',           // Sticky status (not required).
      'revision',         // Skip revisions.
      'log',              // Skip revision logs.
      'language',         // Skip node language (single language site).
      'tnid',             // Translation node ID (not needed).
      'revision_uid',     // Revision author (not necessary).
      'vid',              // Revision ID (auto-handled by destination).
      'comment',          // Skip comment settings.
      'translate',        // Skip translation settings.
    ));
  }

  /**
   * Override the query method to filter published content only.
   */
  protected function query() {
    // Get the default query from DrupalNode7Migration.
    $query = parent::query();

    // Filter to only include published nodes.
    $query->condition('n.status', 1);

    return $query;
  }
}
