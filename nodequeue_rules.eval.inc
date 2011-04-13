<?php
// $Id$

/**
 * @file
 * Contains rules integration for the nodequeue module needed during evaluation.
 *
 * @addtogroup nodequeue
 * @{
 */

/**
 * Condition: Check for selected content types.
 */
function rules_condition_nodequeue_is_named($queue, $name) {
  return $queue->title == $name;
}

/**
 * Add the given node to the given nodequeue.
 */
function rules_action_nodequeue_add_node($node, $sqid, $position) {
  $subqueue = nodequeue_load_subqueue(array_pop($sqid));
  $queue = nodequeue_load($subqueue->qid);
  nodequeue_subqueue_add($queue, $subqueue, $node->nid);
}

/**
 * Remove the given node from the given nodequeue.
 *
 * @see nodequeue_node_delete().
 */  
function rules_action_nodequeue_remove_node($queue, $node) {
  if (!$queue->i18n || (isset($node->tnid) && empty($node->tnid))) {
    nodequeue_subqueue_remove_node($queue->sqid, $node->nid);
  }
}

/**
 * Help for the nodequeue add action.
 */
function nodequeue_rules_rules_add_node_help() {
  return t('');
}

/**
 * Help for the nodequeue remove action.
 */
function nodequeue_rules_rules_remove_node_help() {
  return t('');
}

/**
 * @}
 */
