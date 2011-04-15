<?php
// $Id$

/**
 * @file rules integration for the nodequeue module
 *
 * @addtogroup nodequeue
 * @{
 */

require_once('nodequeue_rules.eval.inc');

/**
 * Implements hook_rules_file_info() on behalf of the nodequeue module.
 */
function nodequeue_rules_rules_file_info() {
  return array('nodequeue_rules.eval');
}

/**
 * Implements hook_rules_event_info() on behalf of the nodequeue module.
 */
function nodequeue_rules_rules_event_info() {
  $items = array(
    'nodequeue_insert' => array(
      'label' => t('After saving new queue'),
      'group' => t('Nodequeue'),
      'variables' => rules_events_nodequeue_variables(t('created queue')),
      'access callback' => 'nodequeue_rules_rules_integration_access',
    ),
    'nodequeue_update' => array(
      'label' => t('After updating existing queue'),
      'group' => t('Nodequeue'),
      'variables' => rules_events_nodequeue_variables(t('updated queue'), TRUE),
      'access callback' => 'nodequeue_rules_rules_integration_access',
    ),
    'nodequeue_presave' => array(
      'label' => t('Before saving queue'),
      'group' => t('Nodequeue'),
      'variables' => rules_events_nodequeue_variables(t('saved queue'), TRUE),
      'access callback' => 'nodequeue_rules_rules_integration_access',
    ),
    'nodequeue_delete' => array(
      'label' => t('After deleting a queue'),
      'group' => t('Nodequeue'),
      'variables' => rules_events_nodequeue_variables(t('deleted queue')),
      'access callback' => 'nodequeue_rules_rules_integration_access',
    ),
  );
  // Specify that on presave the nodequeue is saved anyway.
  $items['nodequeue_presave']['variables']['nodequeue']['skip save'] = TRUE;
  return $items;
}

/**
 * Implements hook_rules_condition_info() on behalf of the nodequeue module.
 */
function nodequeue_rules_rules_condition_info() {
  return array(
    'nodequeue_is_named' => array(
      'label' => t('Nodequeue is named'),
      'help' => t('Evaluates to TRUE if the given queue has the given name.'),
      'base' => 'strstr',//'rules_condition_nodequeue_is_named',
      'group' => t('Nodequeue'),
      'access callback' => 'rules_node_integration_access',
      'parameter' => array(
        'queue' => array(
          'type' => 'list<text>',
          'label' => t('Queue'),
          'options list' => 'nodequeue_rules_rules_get_names',
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info() on behalf of the nodequeue module.
 */
function nodequeue_rules_rules_action_info() {
  return array(
    'nodequeue_add_action' => array(
      'label' => t('Add to Nodequeues'),
      'group' => t('Nodequeue'),
      'callbacks' => array(
        'dependencies' => 'nodequeue_rules_rules_dependencies',
//        'help' => 'nodequeue_rules_rules_add_node_help',
      ),
      'access callback' => 'nodequeue_rules_rules_admin_access',
      'base' => 'rules_action_nodequeue_add_node',
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Node'),
        ),
        'queue' => array(
          'type' => 'list<text>',
          'label' => t('Queue'),
          'options list' => 'nodequeue_rules_rules_get_names',
        ),
        'position' => array(
          'type' => 'integer',
          'label' => t('Position'),
          'optional' => TRUE,
        ),
      ),
    ),
    'nodequeue_remove_action' => array(
      'label' => t('Remove from Nodequeues'),
      'group' => t('Nodequeue'),
      'callbacks' => array(
        'dependencies' => 'nodequeue_rules_rules_dependencies',
//        'help' => 'nodequeue_rules_rules_remove_node_help',
      ),
      'access callback' => 'nodequeue_rules_rules_admin_access',
      'base' => 'rules_action_nodequeue_remove_node',
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Node'),
        ),
        'queue' => array(
          'type' => 'list<text>',
          'label' => t('Queue'),
          'options list' => 'nodequeue_rules_rules_get_names',
        ),
      ),
    ),
  );
}

/**
 * Returns some parameter suitable for using it with a nodequeue
 */
function rules_events_nodequeue_variables($node_label, $update = FALSE) {
  return array(
    'queue' => array(
      'type' => 'text', 
      'label' => $node_label
    ),
  );
}

/**
 * Callback to specify the path module as dependency.
 */
function nodequeue_rules_rules_dependencies() {
  return array('nodequeue');
}

/**
 * Nodequeue integration access callback.
 */
function nodequeue_rules_rules_integration_access($type, $name) {
  if ($type == 'event' || $type == 'condition') {
    return entity_access('view', 'nodequeue');
  }
}

/**
 * Nodequeue integration admin access callback.
 */
function nodequeue_rules_rules_admin_access() {
  return user_access('administer nodequeue');
}

/**
 * Return a list with nodequeue names
 */
function nodequeue_rules_rules_get_names($element, $name = NULL) {
  $items = array();
  $queues = nodequeue_load_queues(nodequeue_get_all_qids(25));
  foreach ($queues as $queue) {
    $items[$queue->qid] = $queue->title;
  }
  return $items;
}


/**
 * @}
 */
