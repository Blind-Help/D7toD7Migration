<?php

/**
 * @file
 * article.inc
 *
 * Migration class for the Article content type
 * 
 * This class extends CommonNodeMigration to inherit common field mappings and exclusions,
 * while also adding specific mappings for the article node type, such as the category field
 */

class ArticleNodeMigration extends CommonNodeMigration {

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->mapArticleFields();
    $this->excludeArticleFields();
  }

  /**
   * Maps article-specific fields
   */
  protected function mapArticleFields() {
    // Map the article category field to the blog category taxonomy from the old site
    $this->mapTaxonomyField(
      DIGITAL_DETOX_FIELD_ARTICLE_CATEGORY,
      DIGITAL_DETOX_FIELD_BLOG_CATEGORY,
      'article_categories'
    );
  }

  /**
   * Excludes fields specific to the article node type that are not needed for migration
   */
  protected function excludeArticleFields() {
    $this->excludeCommonFields(array(
      DIGITAL_DETOX_FIELD_BLOG_CATEGORY . ':language'
    ));
  }
}
