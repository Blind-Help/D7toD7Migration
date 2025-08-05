<?php

/**
 * @file
 * paragraph.inc
 * 
* Contains the paragraph migration classes
 */

// Include required Paragraphs module files
$paragraphs_module_path = drupal_get_path('module', 'paragraphs');
require_once $paragraphs_module_path . '/migrate/destinations/MigrateDestinationParagraphsItem.inc';
require_once $paragraphs_module_path . '/migrate/fields/ParagraphsMigrateParagraphsFieldHandler.inc';

/**
 * Base class for paragraph migrations
 */
abstract class BaseParagraphMigration extends Migration {
  use DigitalDetoxMigrationUtilities;
  use DigitalDetoxParagraphUtilities;

  /**
   * The paragraph bundle type
   */
  protected $bundle;

  /**
   * The destination field name
   */
  protected $destination_field = 'DIGITAL_DETOX_FIELD_NEWS_BODY';

  public function __construct(array $arguments) {
    parent::__construct($arguments);
    $this->setupMigration();
  }

  /**
   * Sets up the complete migration
   */
  protected function setupMigration() {
    $this->source = $this->createSource();
    $this->destination = $this->createDestination();
    $this->map = $this->createMap();
    $this->setupFieldMappings();
  }

  /**
   * Creates the migration source
   */
  protected function createSource() {
    $query = $this->buildSourceQuery();
    $source_fields = $this->getSourceFields();
    
    return new MigrateSourceSQL($query, $source_fields, NULL, array('map_joinable' => FALSE));
  }

  /**
   * Creates the migration destination
   */
  protected function createDestination() {
    return new MigrateDestinationParagraphsItem($this->bundle, array(
      'field_name' => $this->destination_field,
      'language' => NULL,
      'text_format' => DIGITAL_DETOX_DEFAULT_TEXT_FORMAT,
    ));
  }

  /**
   * Creates the migration map
   */
  protected function createMap() {
    return new MigrateSQLMap(
      $this->machineName,
      array(
        'item_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Unique paragraph item ID',
        ),
      ),
      MigrateDestinationParagraphsItem::getKeySchema()
    );
  }

  /**
   * Builds the source query
   */
  protected function buildSourceQuery() {
    $query = Database::getConnection('default', DIGITAL_DETOX_SOURCE_CONNECTION)
      ->select('paragraphs_item', 'pi')
      ->fields('pi', array('item_id', 'bundle', 'field_name'))
      ->condition('pi.bundle', $this->bundle)
      ->condition('pi.item_id', 0, '>')
      ->orderBy('pi.item_id');

    $this->addBundleJoins($query);
    return $query;
  }

  /**
   * Gets source fields
   */
  protected function getSourceFields() {
    $fields = array(
      'item_id' => t('Paragraph Item ID'),
      'bundle' => t('Paragraph Bundle'),
      'field_name' => t('Field Name'),
    );
    
    $this->addBundleFields($fields);
    return $fields;
  }

  /**
   * Sets up field mappings
   */
  protected function setupFieldMappings() {
    $this->addFieldMapping('field_name', 'field_name');
    $this->addFieldMapping('bundle', 'bundle');
    $this->addBundleMappings();
  }

  /**
   * Validates row data
   */
  public function prepareRow($row) {
    return $this->validateParagraphRow($row) ? parent::prepareRow($row) : FALSE;
  }

  /**
   * Prevents stub creation
   */
  protected function createStub($migration, array $source_id) {
    return FALSE;
  }

  // Abstract methods for subclasses
  abstract protected function addBundleJoins($query);
  abstract protected function addBundleFields(&$fields);
  abstract protected function addBundleMappings();
}

/**
 * Migration class for content paragraphs
 */
class ContentParagraphMigration extends BaseParagraphMigration {
  
  protected $bundle = DIGITAL_DETOX_PARAGRAPH_BUNDLE_CONTENT;
  protected $source_migrations = array('article', 'game', 'software');

  public function __construct(array $arguments) {
    parent::__construct($arguments);
    $this->description = t('Migrate content paragraphs to the new structure.');
  }

  protected function addBundleJoins($query) {
    $query->leftJoin('field_data_' . DIGITAL_DETOX_PARAGRAPH_FIELD_TITLE, 'ft', 
      'ft.entity_id = pi.item_id AND ft.entity_type = :entity_type', 
      array(':entity_type' => 'paragraphs_item'));
    $query->addField('ft', DIGITAL_DETOX_PARAGRAPH_FIELD_TITLE . '_value');
  }

  protected function addBundleFields(&$fields) {
    $fields[DIGITAL_DETOX_PARAGRAPH_FIELD_TITLE . '_value'] = t('Title');
  }

  protected function addBundleMappings() {
    $this->addFieldMapping(DIGITAL_DETOX_PARAGRAPH_DEST_TITLE, DIGITAL_DETOX_PARAGRAPH_FIELD_TITLE . '_value');
    $this->addFieldMapping(DIGITAL_DETOX_PARAGRAPH_DEST_CONTENT, 'field_content');
  }

  public function prepareRow($row) {
    if (!parent::prepareRow($row)) {
      return FALSE;
    }

    $row->field_content = $this->fetchContentReferences($row->item_id);
    return TRUE;
  }

  public function prepare($entity, stdClass $row) {
    if (empty($row->field_content)) {
      return;
    }

    $migrated_ids = $this->handleContentReferences($row->field_content, $this->source_migrations);
    
    if (!empty($migrated_ids)) {
      $entity->field_pc_content = $this->createParagraphFieldArray($migrated_ids);
    }
  }
}

/**
 * Migration class for text paragraphs
 */
class TextParagraphMigration extends BaseParagraphMigration {
  
  protected $bundle = DIGITAL_DETOX_PARAGRAPH_BUNDLE_TEXT;

  public function __construct(array $arguments) {
    parent::__construct($arguments);
    $this->description = t('Migrate text paragraphs to the new structure.');
  }

  protected function addBundleJoins($query) {
    $query->leftJoin('field_data_' . DIGITAL_DETOX_PARAGRAPH_FIELD_DESCRIPTION, 'fd', 
      'fd.entity_id = pi.item_id AND fd.entity_type = :entity_type', 
      array(':entity_type' => 'paragraphs_item'));
    $query->addField('fd', DIGITAL_DETOX_PARAGRAPH_FIELD_DESCRIPTION . '_value');
  }

  protected function addBundleFields(&$fields) {
    $fields[DIGITAL_DETOX_PARAGRAPH_FIELD_DESCRIPTION . '_value'] = t('Description');
  }

  protected function addBundleMappings() {
    $this->addFieldMapping(DIGITAL_DETOX_PARAGRAPH_DEST_TEXT, DIGITAL_DETOX_PARAGRAPH_FIELD_DESCRIPTION . '_value');
    $this->addFieldMapping(DIGITAL_DETOX_PARAGRAPH_DEST_TEXT . ':format')
      ->defaultValue(DIGITAL_DETOX_DEFAULT_TEXT_FORMAT);
  }

  public function prepareRow($row) {
    if (!parent::prepareRow($row)) {
      return FALSE;
    }

    $field_value = DIGITAL_DETOX_PARAGRAPH_FIELD_DESCRIPTION . '_value';
    if (!$this->validateParagraphField($row->$field_value)) {
      $this->logMigrationError('Empty text content for paragraph item_id @item_id', 
        array('@item_id' => $row->item_id));
      return FALSE;
    }

    return TRUE;
  }
}

/**
 * Migration class for video paragraphs
 */
class VideoParagraphMigration extends BaseParagraphMigration {
  
  protected $bundle = DIGITAL_DETOX_PARAGRAPH_BUNDLE_VIDEO;

  public function __construct(array $arguments) {
    parent::__construct($arguments);
    $this->description = t('Migrate video paragraphs to the new structure.');
  }

  protected function addBundleJoins($query) {
    $query->leftJoin('field_data_' . DIGITAL_DETOX_PARAGRAPH_FIELD_VIDEO_LINK, 'fv', 
      'fv.entity_id = pi.item_id AND fv.entity_type = :entity_type', 
      array(':entity_type' => 'paragraphs_item'));
    $query->addField('fv', DIGITAL_DETOX_PARAGRAPH_FIELD_VIDEO_LINK . '_input');
  }

  protected function addBundleFields(&$fields) {
    $fields[DIGITAL_DETOX_PARAGRAPH_FIELD_VIDEO_LINK . '_input'] = t('Video Link Input');
  }

  protected function addBundleMappings() {
    $this->addFieldMapping(DIGITAL_DETOX_PARAGRAPH_DEST_VIDEO, DIGITAL_DETOX_PARAGRAPH_FIELD_VIDEO_LINK . '_input');
  }

  public function prepareRow($row) {
    if (!parent::prepareRow($row)) {
      return FALSE;
    }

    $field_value = DIGITAL_DETOX_PARAGRAPH_FIELD_VIDEO_LINK . '_input';
    
    if (!$this->validateParagraphField($row->$field_value)) {
      $this->logMigrationError('Empty video link for paragraph item_id @item_id', 
        array('@item_id' => $row->item_id));
      return FALSE;
    }

    if (!$this->isValidYouTubeLink($row->$field_value)) {
      $this->logMigrationError('Invalid YouTube link for paragraph item_id @item_id: @link', 
        array('@item_id' => $row->item_id, '@link' => $row->$field_value));
      return FALSE;
    }

    return TRUE;
  }
}
