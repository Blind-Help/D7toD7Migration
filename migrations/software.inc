<?php

/**
 * @file
 * software.inc
 *
 * Migration class for the Software content type.
 * 
 * This class extends CommonNodeMigration to inherit common field mappings and exclusions,
 * while also adding specific mappings for the software node type, such as OS and category fields.
 */

class SoftwareNodeMigration extends CommonNodeMigration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Map the operating system field to the OS taxonomy from the old site.
    $this->addFieldMapping('field_software_os', 'field_operating_system')->sourceMigration('operating_systems');
    $this->addFieldMapping('field_software_os:source_type')->defaultValue('tid');
    $this->addFieldMapping('field_software_os:ignore_case')->defaultValue(TRUE);

    // Map the software category field to the category taxonomy from the old site.
    $this->addFieldMapping('field_software_category', 'field_category')->sourceMigration('categories');
    $this->addFieldMapping('field_software_category:source_type')->defaultValue('tid');
    $this->addFieldMapping('field_software_category:ignore_case')->defaultValue(TRUE);

    // Map the download link field and its attributes.
    $this->addFieldMapping('field_software_dl', 'field_download_link');
    $this->addFieldMapping('field_software_dl:title', 'field_download_link:title');
    $this->addFieldMapping('field_software_dl:attributes', 'field_download_link:attributes');

    // Exclude fields that are not needed for migration.
    $this->addUnmigratedSources(array(
      'field_image',
      'field_image:alt',
      'field_image:title',
      'field_image:width',
      'field_image:height',
      'field_image:language',
      'field_notes',
      'field_notes:language',
      'field_update_item',
      'field_update_item:language',
      'field_content_approval',
      'field_content_approval:language',
      'field_operating_system:language',
      'field_category:language',
      'field_download_link:language',
      'field_tags',
      'field_tags:language',
    ));
  }
}
