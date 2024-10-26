<?php

/**
 * @file
 * article.inc
 *
 * Migration class for the Article content type.
 * 
 * This class extends CommonNodeMigration to inherit common field mappings and exclusions,
 * while also adding specific mappings for the article node type, such as the category field.
 */

class ArticleNodeMigration extends CommonNodeMigration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Map the article category field to the blog category taxonomy from the old site.
    $this->addFieldMapping('field_article_category', 'field_blog_category')
      ->sourceMigration('article_categories');
    $this->addFieldMapping('field_article_category:source_type')
      ->defaultValue('tid');
    $this->addFieldMapping('field_article_category:ignore_case')
      ->defaultValue(TRUE);

    // Exclude fields specific to the article node type that are not needed for migration.
    $this->addUnmigratedSources(array(
      'field_tags',
      'field_tags:language',
      'field_blog_category:language',
      'field_image',
      'field_image:alt',
      'field_image:title',
      'field_image:width',
      'field_image:height',
      'field_image:language',
      'field_content_approval',
      'field_content_approval:language',
    ));
  }
}
