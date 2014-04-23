<?php

/**
 * Implements hook_migrate_api().
 */
function ucsf_it_migration_migrate_api() {

  // common arguments shared by all migrations
  $common_arguments = array(
    'source_version' => 6,
    'group_name' => 'ucsf_it',
    'source_connection' => 'legacy',
  );

  $api = array(
    'api' => 2,
    // define migration group
    'groups' => array(
      'ucsf_it' => array(
        'title' => t("UCSF IT Website Migrations"),
      ),
    ),
    'migrations' => array(
      // register user role migration
      'D6Role' => $common_arguments + array(
        'class_name' => 'DrupalRole6Migration',
        'description' => t('Migration of users from Drupal 6'),
        'role_mappings' => array(
          'admin' => 'administrator',
        ),
      ),
      // register user migration
      'D6User' => $common_arguments + array(
        'class_name' => 'DrupalUser6Migration',
        'description' => t('Migration of users from Drupal 6'),
        'dependencies' => array('D6Role'),
        'role_migration' => 'D6Role',
      ),
    ),
  );

  // register vocabulary migrations
  $common_vocabulary_arguments = $common_arguments + array(
    'class_name' => 'UcsfItTerm6Migration',
    'soft_dependencies' => array('D6User'),
  );

  $vocabulary_arguments = array(
    'D6ItsCategoriesVocabulary' => array(
      'description' => t('Migration of ITS categories from Drupal 6'),
      'source_vocabulary' => '2',  // D6 vocabulary id
      'destination_vocabulary' => 'its_categories', // D7 vocabulary machine name
    ),
    'D6NewsTermsVocabulary' => array(
      'description' => t('Migration of News terms from Drupal 6'),
      'source_vocabulary' => '6',
      'destination_vocabulary' => 'news_terms',
    ),
  );

  foreach ($vocabulary_arguments as $key => $arguments) {
    $arguments += $common_vocabulary_arguments;
    $api['migrations'][$key] = $arguments;
  }

  // register file migration
  $api['migrations']['D6File'] = $common_arguments + array(
    'description' => t('Migration of files from Drupal 6'),
    'class_name' => 'DrupalFile6Migration',
    'user_migration' => 'D6User',
    'default_uid' => 1,
    // KLUDGE!
    // Hardwired path to my local sites directory in the D6 environment.
    // @todo See if this can be made more portable. [ST 2014/04/02]
    'source_dir' => '/home/stefan/dev/projects/ucsf/docroot/sites/it.ucsf.edu/files',
  );

  // register node migrations
  $node_arguments = $common_arguments + array(
    'user_migration' => 'D6User',
    'default_uid' => 0,
  );

  $api['migrations']['D6StoryNode'] = $node_arguments + array(
    'source_type' => 'story',
    'destination_type' => 'article',
    'description' => t('Migration of Story nodes from Drupal 6'),
    'class_name' => 'UcsfItStoryNode6Migration',
  );

  $api['migrations']['D6StatusNode'] = $node_arguments + array(
    'source_type' => 'it_outage',
    'destination_type' => 'it_outage',
    'description' => t('Migration of IT Status nodes from Drupal 6'),
    'class_name' => 'UcsfItStatusNode6Migration',
    'dependencies' => array('D6ItsCategoriesVocabulary'),
  );

  $api['migrations']['D6PageNode'] = $node_arguments + array(
    'source_type' => 'page',
    'destination_type' => 'page',
    'description' => t('Migration of Page nodes from Drupal 6'),
    'class_name' => 'UcsfItPageNode6Migration',
    'dependencies' => array('D6File'),
  );

  $api['migrations']['D6NewsNode'] = $node_arguments + array(
    'source_type' => 'it_news',
    'destination_type' => 'it_news',
    'description' => t('Migration of IT News nodes from Drupal 6'),
    'class_name' => 'UcsfItNewsNode6Migration',
    'dependencies' => array('D6File', 'D6NewsTermsVocabulary'),
  );

  $api['migrations']['D6HowDoNode'] = $node_arguments + array(
    'source_type' => 'it_how_to',
    'destination_type' => 'it_how_do',
    'description' => t('Migration of IT How Do nodes from Drupal 6'),
    'class_name' => 'UcsfItHowDoNode6Migration',
    'dependencies' => array('D6File', 'D6ItsCategoriesVocabulary'),
  );

  $api['migrations']['D6ServiceNode'] = $node_arguments + array(
    'source_type' => 'it_service',
    'destination_type' => 'it_service',
    'description' => t('Migration of IT Service nodes from Drupal 6'),
    'class_name' => 'UcsfItServiceNode6Migration',
    'dependencies' => array('D6File', 'D6ItsCategoriesVocabulary'),
  );

  $api['migrations']['D6AdditionalInfoNode'] = $node_arguments + array(
    'source_type' => 'it_additional_information',
    'destination_type' => 'it_additional_information',
    'description' => t('Migration of IT Service Additional Information nodes from Drupal 6'),
    'class_name' => 'UcsfItServiceAdditionalInfoNode6Migration',
    'dependencies' => array('D6File', 'D6ServiceNode'),
  );

  $api['migrations']['D6TutorialNode'] = $node_arguments + array(
    'source_type' => 'it_service_tutorial',
    'destination_type' => 'it_service_tutorial',
    'description' => t('Migration of IT Service Tutorial nodes from Drupal 6'),
    'class_name' => 'UcsfItServiceTutorialNode6Migration',
    'dependencies' => array('D6File', 'D6ServiceNode'),
  );

  $api['migrations']['D6CategoryDescriptionNode'] = $node_arguments + array(
    'source_type' => 'it_category_description',
    'destination_type' => 'it_category_description',
    'description' => t('Migration of IT Category Description nodes from Drupal 6'),
    'class_name' => 'UcsfItCategoryDescriptionNode6Migration',
    'dependencies' => array('D6File', 'D6ItsCategoriesVocabulary'),
  );

  $api['migrations']['D6ProjectNode'] = $node_arguments + array(
    'source_type' => 'it_project',
    'destination_type' => 'it_project',
    'description' => t('Migration of IT Project nodes from Drupal 6'),
    'class_name' => 'UcsfItProjectNode6Migration',
    'dependencies' => array('D6File', 'D6ItsCategoriesVocabulary', 'D6ServiceNode'),
  );

  $api['migrations']['D6ProjectStatusNode'] = $node_arguments + array(
    'source_type' => 'it_project_status',
    'destination_type' => 'it_project_status',
    'description' => t('Migration of IT Project Status nodes from Drupal 6'),
    'class_name' => 'UcsfItProjectStatusNode6Migration',
    'dependencies' => array('D6File', 'D6ProjectNode'),
  );

  $api['migrations']['D6TeamNode'] = $node_arguments + array(
    'source_type' => 'it_team',
    'destination_type' => 'it_team',
    'description' => t('Migration of IT Team nodes from Drupal 6'),
    'class_name' => 'UcsfItTeamNode6Migration',
    'dependencies' => array('D6File', 'D6ItsCategoriesVocabulary'),
  );

  $api['migrations']['D6PolicyNode'] = $node_arguments + array(
    'source_type' => 'policy_standard',
    'destination_type' => 'policy_or_standard',
    'description' => t('Migration of IT Policy Or Standard nodes from Drupal 6'),
    'class_name' => 'UcsfItPolicyOrStandardNode6Migration',
    'dependencies' => array('D6File', 'D6ItsCategoriesVocabulary'),
  );

  $api['migrations']['D6Redirect'] = $node_arguments + array(
      'description' => t('Migration of path redirects.'),
      'class_name' => 'UcsfItRedirectMigration',
    );

  return $api;
}
