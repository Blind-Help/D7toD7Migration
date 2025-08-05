<?php

/**
 * @file
 * digital_detox.migrate.inc
 *
 * Implements hook_migrate_api() for the Digital Detox module
 */

function digital_detox_migrate_api() {
  // Declare the API version and migration group
  $api = array(
    'api' => 2,
    'groups' => array(
      DIGITAL_DETOX_MIGRATION_GROUP => array(
        'title' => t('Detox Content & Terms from Old Site'),
        'description' => t('Migrate content and taxonomy from the legacy system to the new structure.'),
      ),
    ),
    'migrations' => array(),
  );

  // Common arguments used across all migrations
  $common_arguments = array(
    'source_connection' => DIGITAL_DETOX_SOURCE_CONNECTION,
    'source_version' => DIGITAL_DETOX_SOURCE_VERSION,
    'group_name' => DIGITAL_DETOX_MIGRATION_GROUP,
  );

  // Node migration arguments for 'Article', 'Basic page', 'Game', 'News', 'Software', and 'Voice' content types
  $node_arguments = array(
    'article' => array(
      'class_name' => 'ArticleNodeMigration',
      'description' => t('Migrate article content with category taxonomy mapping.'),
      'source_type' => 'article',
      'destination_type' => 'article',
    ),
    'page' => array(
      'class_name' => 'PageNodeMigration',
      'description' => t('Migrate basic page content to the new structure.'),
      'source_type' => 'page',
      'destination_type' => 'page',
    ),
    'game' => array(
      'class_name' => 'GameNodeMigration',
      'description' => t('Migrate game content with platform taxonomy and download link mapping.'),
      'source_type' => 'software',
      'destination_type' => 'game',
    ),
    'software' => array(
      'class_name' => 'SoftwareNodeMigration',
      'description' => t('Migrate software content with OS and category taxonomy mapping.'),
      'source_type' => 'software',
      'destination_type' => 'software',
    ),
    'news' => array(
      'class_name' => 'NewsNodeMigration',
      'description' => t('Migrate news content with paragraph field mapping.'),
      'source_type' => 'news',
      'destination_type' => 'news',
    ),
    'voice' => array(
      'class_name' => 'VoiceNodeMigration',
      'description' => t('Migrate voice content with language taxonomy mapping.'),
      'source_type' => 'text_to_speech_voice',
      'destination_type' => 'voice',
    ),
  );

  // Declare node migrations
  foreach ($node_arguments as $migration_name => $arguments) {
    $arguments = array_merge_recursive($arguments, $common_arguments);
    $api['migrations'][$migration_name] = $arguments;
  }

  // Taxonomy migration arguments for 'Article categories', 'Software operating systems', 'Software categories', and 'Voice Languages' vocabulary
  $term_arguments = array(
    'article_categories' => array(
      'class_name' => 'TermMigration',
      'description' => t('Migrate article category taxonomy terms with name formatting.'),
      'source_vocabulary' => 'blog_categories',
      'destination_vocabulary' => 'article_categories',
    ),
    'operating_systems' => array(
      'class_name' => 'TermMigration',
      'description' => t('Migrate operating system taxonomy terms for software classification.'),
      'source_vocabulary' => 'os',
      'destination_vocabulary' => 'software_operating_systems',
    ),
    'categories' => array(
      'class_name' => 'TermMigration',
      'description' => t('Migrate software category taxonomy terms with name formatting.'),
      'source_vocabulary' => 'categories',
      'destination_vocabulary' => 'software_categories',
    ),
    'voice_languages' => array(
      'class_name' => 'TermMigration',
      'description' => t('Migrate voice language taxonomy terms with proper formatting.'),
      'source_vocabulary' => 'text_to_speech_languages',
      'destination_vocabulary' => 'voice_languages',
    ),
  );

  // Declare taxonomy term migrations
  foreach ($term_arguments as $migration_name => $arguments) {
    $arguments = array_merge_recursive($arguments, $common_arguments);
    $api['migrations'][$migration_name] = $arguments;
  }

  // User migration arguments
  $user_arguments = array(
    'user' => array(
      'class_name' => 'UserMigration',
      'description' => t('Migrate user accounts with essential authentication data.'),
    ),
  );

  // Declare user migration
  foreach ($user_arguments as $migration_name => $arguments) {
    $arguments = array_merge_recursive($arguments, $common_arguments);
    $api['migrations'][$migration_name] = $arguments;
  }

// Paragraph migration arguments
$paragraph_arguments = array(
  'paragraph_content' => array(
    'class_name' => 'ContentParagraphMigration',
  ),
  'paragraph_text' => array(
    'class_name' => 'TextParagraphMigration',
  ),
  'paragraph_video' => array(
    'class_name' => 'VideoParagraphMigration',
  ),
);

// Register paragraph migration
foreach ($paragraph_arguments as $migration_name => $arguments) {
  $api['migrations'][$migration_name] = array_merge_recursive($arguments, $common_arguments);
}

  return $api;
}
