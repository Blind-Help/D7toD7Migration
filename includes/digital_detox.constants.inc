<?php

/**
 * @file
 * digital_detox.constants.inc
 *
 * Constants and configuration values for the Digital Detox migration module
 */

/**
 * Source system taxonomy term IDs
 */
define('DIGITAL_DETOX_GAME_TERM_ID', 4);

/**
 * Default values for migrations
 */
define('DIGITAL_DETOX_DEFAULT_USER_ID', 1);
define('DIGITAL_DETOX_DEFAULT_TEXT_FORMAT', 'full_html');

/**
 * Migration group configuration
 */
define('DIGITAL_DETOX_MIGRATION_GROUP', 'content_migration');

/**
 * Database connection configuration
 */
define('DIGITAL_DETOX_SOURCE_CONNECTION', 'old_db');
define('DIGITAL_DETOX_SOURCE_VERSION', 7);

/**
 * Source field names from the old system
 */
define('DIGITAL_DETOX_FIELD_OPERATING_SYSTEM', 'field_operating_system');
define('DIGITAL_DETOX_FIELD_DOWNLOAD_LINK', 'field_download_link');
define('DIGITAL_DETOX_FIELD_CATEGORY', 'field_category');
define('DIGITAL_DETOX_FIELD_BLOG_CATEGORY', 'field_blog_category');
define('DIGITAL_DETOX_FIELD_NEWS_BODY', 'field_news_body');
define('DIGITAL_DETOX_FIELD_NOTIFICATION_ID', 'field_notification_id');
define('DIGITAL_DETOX_FIELD_TTS_LANGUAGE', 'field_tts_language');
define('DIGITAL_DETOX_FIELD_GENDER', 'field_gender');

/**
 * Destination field names for the new system
 */
define('DIGITAL_DETOX_FIELD_SOFTWARE_OS', 'field_software_os');
define('DIGITAL_DETOX_FIELD_SOFTWARE_CATEGORY', 'field_software_category');
define('DIGITAL_DETOX_FIELD_SOFTWARE_DL', 'field_software_dl');
define('DIGITAL_DETOX_FIELD_ARTICLE_CATEGORY', 'field_article_category');
define('DIGITAL_DETOX_FIELD_NEWS_ID', 'field_news_id');
define('DIGITAL_DETOX_FIELD_VOICE_LANGUAGE', 'field_voice_language');
define('DIGITAL_DETOX_FIELD_VOICE_SYNTHESIZER', 'field_voice_synthesizer');
define('DIGITAL_DETOX_FIELD_VOICE_GENDER', 'field_voice_gender');

/**
 * Paragraph field constants for consistent field mapping
 */
define('DIGITAL_DETOX_PARAGRAPH_FIELD_TITLE', 'field_title');
define('DIGITAL_DETOX_PARAGRAPH_FIELD_CONTENT', 'field_content');
define('DIGITAL_DETOX_PARAGRAPH_FIELD_DESCRIPTION', 'field_discription');
define('DIGITAL_DETOX_PARAGRAPH_FIELD_VIDEO_LINK', 'field_video_link');

/**
 * Destination field constants for new paragraph structure
 */
define('DIGITAL_DETOX_PARAGRAPH_DEST_TITLE', 'field_pc_title');
define('DIGITAL_DETOX_PARAGRAPH_DEST_CONTENT', 'field_pc_content');
define('DIGITAL_DETOX_PARAGRAPH_DEST_TEXT', 'field_pt_text');
define('DIGITAL_DETOX_PARAGRAPH_DEST_VIDEO', 'field_pv_yt_link');

/**
 * Paragraph bundle types
 */
define('DIGITAL_DETOX_PARAGRAPH_BUNDLE_CONTENT', 'content');
define('DIGITAL_DETOX_PARAGRAPH_BUNDLE_TEXT', 'text');
define('DIGITAL_DETOX_PARAGRAPH_BUNDLE_VIDEO', 'video');

/**
 * Common excluded fields to prevent migration failures
 */
define('DIGITAL_DETOX_EXCLUDED_IMAGE_FIELDS', array(
  'field_image',
  'field_image:alt',
  'field_image:title',
  'field_image:width',
  'field_image:height',
  'field_image:language'
));

define('DIGITAL_DETOX_EXCLUDED_COMMON_FIELDS', array(
  'field_notes',
  'field_notes:language',
  'field_update_item',
  'field_update_item:language',
  'field_content_approval',
  'field_content_approval:language',
  'field_tags',
  'field_tags:language'
));
