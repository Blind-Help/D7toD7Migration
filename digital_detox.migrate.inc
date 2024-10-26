<?php

/**
 * @file
 * digital_detox.migrate.inc
 *
 * Implements hook_migrate_api() for the Digital Detox module.
 */
function digital_detox_migrate_api() {
  // Declare the API version and migration group.
  $api = array(
    'api' => 2,
    'groups' => array(
      'content_migration' => array(
        'title' => t('Detox Content & Terms from Old Site'),
        'description' => t('The ultimate digital cleanse—purging old content and taxonomy from a site stuck in 2014.'),
      ),
    ),
    'migrations' => array(),
  );

  // Common arguments used across all migrations.
  $common_arguments = array(
    'source_connection' => 'old_db',
    'source_version' => 7,
    'group_name' => 'content_migration',
  );

  // Node migration arguments for 'Article', 'Basic page', and 'Software' content types.
  $node_arguments = array(
    'article' => array(
      'class_name' => 'ArticleNodeMigration',
      'description' => t('Giving your old, dusty articles a one-way ticket to relevance on the new site. Because 8 years is practically prehistoric in internet years.'),
      'source_type' => 'article',
      'destination_type' => 'article',
    ),
    'page' => array(
      'class_name' => 'PageNodeMigration',
      'description' => t('Pages so old, they could be historical artifacts. We\'re bringing them back—refreshed and ready for a new world.'),
      'source_type' => 'page',
      'destination_type' => 'page',
    ),
    'software' => array(
      'class_name' => 'SoftwareNodeMigration',
      'description' => t('Dragging your software nodes kicking and screaming into the modern era, with fewer bugs and more features (hopefully).'),
      'source_type' => 'software',
      'destination_type' => 'software',
    ),
  );

  // Declare node migrations.
  foreach ($node_arguments as $migration_name => $arguments) {
    $arguments = array_merge_recursive($arguments, $common_arguments);
    $api['migrations'][$migration_name] = $arguments;
  }

  // Taxonomy migration arguments for 'Article categories', 'Software operating system', and 'Software categories.'
  $term_arguments = array(
    'article_categories' => array(
      'class_name' => 'TermMigration',
      'description' => t('Your blog categories are moving into the present, one term at a time. Just don’t expect them to be hip.'),
      'source_vocabulary' => 'blog_categories',
      'destination_vocabulary' => 'article_categories',
    ),
    'operating_systems' => array(
      'class_name' => 'TermMigration',
      'description' => t('From outdated to optimized: migrating operating systems for software on the new site.'),
      'source_vocabulary' => 'os',
      'destination_vocabulary' => 'software_operating_systems',
    ),
    'categories' => array(
      'class_name' => 'TermMigration',
      'description' => t('Taking your old categories, giving them a makeover, and pretending they’ve always looked this good. You’re welcome.'),
      'source_vocabulary' => 'categories',
      'destination_vocabulary' => 'software_categories',
    ),
  );

  // Declare taxonomy term migrations.
  foreach ($term_arguments as $migration_name => $arguments) {
    $arguments = array_merge_recursive($arguments, $common_arguments);
    $api['migrations'][$migration_name] = $arguments;
  }

  return $api;
}
