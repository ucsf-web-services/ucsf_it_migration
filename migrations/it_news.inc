<?php

/**
 * @file it_news.inc
 */

/**
 * Node migration class for Drupal 6.
 * Handles the migration of "IT News" nodes from D6 to D7.
 *
 * @see UcsfItNode6Migration
 */
class UcsfItNewsNode6Migration extends UcsfItNode6Migration {

  public function __construct(array $arguments) {

    $this->sourceFields['ref_nids'] = 'NIDs of referenced nodes.';

    parent::__construct($arguments);

    // map image and attachment fields
    // @link https://drupal.org/node/1819704
    // @link http://drupal.stackexchange.com/a/58637/1801
    $this->addFieldMapping('field_it_news_image', 'image_fid')->sourceMigration('D6File');
    $this->addFieldMapping('field_it_news_image:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_it_news_image:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_it_news_image:language')->defaultValue(LANGUAGE_NONE);
    $this->addFieldMapping('field_it_news_attachments', 'attachment_fids')->separator(',')->sourceMigration('D6File');
    $this->addFieldMapping('field_it_news_attachments:file_class')->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_it_news_attachments:preserve_files')->defaultValue(TRUE);
    $this->addFieldMapping('field_it_news_attachments:language')->defaultValue(LANGUAGE_NONE);
    // map taxonomy terms
    $this->addFieldMapping('field_it_news_taxonomy', '6')->sourceMigration('D6NewsTermsVocabulary');
    $this->addFieldMapping('field_it_news_taxonomy:source_type')->defaultValue('tid');
    $this->addFieldMapping('field_it_news_taxonomy:create_term')->defaultValue(FALSE);
    $this->addFieldMapping('field_it_news_taxonomy:ignore_case')->defaultValue(FALSE);
    // map node references
    $this->addFieldMapping('field_it_news_related_page', 'ref_nids');

    // DNM
    $this->addUnmigratedSources(array('field_attached_file', 'field_attached_file:list', 'field_news_image',
      'field_news_image:list', 'field_news_image:description', 'field_related_page', 'related_nids'));
    $this->addUnmigratedDestinations(array('field_it_news_attachments:description', 'field_it_news_attachments:display',
      'field_it_news_image:alt', 'field_it_news_image:title'));
  }

  /**
   * @see DrupalNodeMigration::query()
   * @return SelectQueryInterface
   */
  public function query() {
    $query = parent::query();
    // get the file-ids for associated images and attachments here
    $query->leftJoin('content_field_news_image', 'cfni', 'cfni.vid = n.vid');
    $query->addField('cfni', 'field_news_image_fid', 'image_fid');
    $query->leftJoin('content_field_attached_file', 'cfa', 'cfa.vid = n.vid');
    $query->addExpression('GROUP_CONCAT(DISTINCT cfa.field_attached_file_fid)', 'attachment_fids');
    // get nids for related nodes.
    $query = $this->addRelatedNodesToQuery($query);
    $query->groupBy('n.vid');
    return $query;
  }

  /**
   * @param $row
   * @return bool
   *
   * @see DrupalNodeMigration::prepareRow()
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    $ref_nids = $this->processRelatedNids($row->related_nids);
    if ($ref_nids) {
      $row->ref_nids = $ref_nids;
    }
  }
}
