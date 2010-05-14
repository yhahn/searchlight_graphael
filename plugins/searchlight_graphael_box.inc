<?php
/**
 * Simple custom text box.
 */
class searchlight_graphael_box extends boxes_box {
  protected function get_datasource() {
    $base_table = array_pop(explode('searchlight_graphael_', $this->plugin_key));
    return searchlight_get_datasource($base_table);
  }

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    return array(
      'method' => 'pie',
      'facet' => '',
      'items' => 20,
    );
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form() {
    $form = array();
    $form['method'] = array(
      '#title' => t('Graph type'),
      '#options' => array(
        'pie' => t('Pie'),
        'bar' => t('Bar'),
        'line' => t('Line'),
        'dot' => t('Dot'),
      ),
      '#type' => 'select',
      '#default_value' => $this->options['method'],
    );

    // Generate facet options.
    $options = array();
    if ($datasource = $this->get_datasource()) {
      foreach ($datasource->fields as $name => $field) {
        if ($field['usage'] === 'attribute' || $field['usage'] === 'multivalue') {
          $options[$name] = $field['label'];
        }
      }
    }
    $form['facet'] = array(
      '#title' => t('Facet'),
      '#options' => $options,
      '#type' => 'select',
      '#default_value' => $this->options['facet'],
    );

    // Items to be shown.
    $form['items'] = array(
      '#title' => t('Items'),
      '#options' => range(1, 20),
      '#type' => 'select',
      '#default_value' => $this->options['items'],
    );
    return $form;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function render() {
    if ($datasource = $this->get_datasource()) {
      $attr = array('style' => 'width:auto; height:200px;');
      $graph = searchlight_graphael_build($this->options['method'], $datasource, $this->options['facet'], $this->options['items']);
      $content = theme('graphael', $graph['method'], $graph['values'], $graph['params'], $graph['extend'], $attr);
      $title = isset($this->title) ? check_plain($this->title) : NULL;
      return array(
        'delta' => $this->delta, // Crucial.
        'title' => $title,
        'subject' => $title,
        'content' => $content,
      );
    }
  }
}
