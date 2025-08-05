<?php

/**
 * @file
 * voice.inc
 *
 * Migration class for voice nodes
 * 
 * This class extends CommonNodeMigration to inherit common field mappings and exclusions
 */

class VoiceNodeMigration extends CommonNodeMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Map voice-specific fields
    $this->mapVoiceFields();
    
    // Exclude voice-specific fields
    $this->excludeVoiceFields();
  }

  /**
   * Maps voice-specific fields with proper transformations
   */
  protected function mapVoiceFields() {
    // Map language taxonomy field
    $this->mapTaxonomyField(
      DIGITAL_DETOX_FIELD_VOICE_LANGUAGE,
      DIGITAL_DETOX_FIELD_TTS_LANGUAGE,
      'voice_languages'
    );

    // Map synthesizer field (entity ref → text list with default value)
    $this->addFieldMapping(DIGITAL_DETOX_FIELD_VOICE_SYNTHESIZER)
      ->defaultValue('ve2_nvda');

    // Map gender field
    $this->addFieldMapping(DIGITAL_DETOX_FIELD_VOICE_GENDER, DIGITAL_DETOX_FIELD_GENDER);

    // Map download link field
    $this->mapLinkField(DIGITAL_DETOX_FIELD_SOFTWARE_DL, DIGITAL_DETOX_FIELD_DOWNLOAD_LINK);
  }

  /**
   * Excludes voice-specific fields that should not be migrated
   */
  protected function excludeVoiceFields() {
    $this->excludeCommonFields();
  }

}
