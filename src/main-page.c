#include <pebble.h>

#define APP_RUN_COUNTER_PKEY 1

static Window *s_main_window;
static TextLayer *s_output_layer;

static void up_click_handler() {
  text_layer_set_text(s_output_layer, "Up pressed!");
}

static void select_click_handler() {
	static char counter_display[3];
	snprintf( counter_display, sizeof( counter_display ), "%u", (int) persist_read_int( APP_RUN_COUNTER_PKEY ) );
  text_layer_set_text( s_output_layer, counter_display );
}

static void down_click_handler() {
  text_layer_set_text(s_output_layer, "Down pressed!");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}


static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  s_output_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 5, window_bounds.size.h));
  
  text_layer_set_text_alignment(s_output_layer, GTextAlignmentRight);
  
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
}

static void main_window_unload(Window *window) {
  // Destroy Window's child Layers here
}

static void increment_app_run_counter(void) {
	text_layer_set_text(s_output_layer, "Press a button.");
	if ( persist_exists( APP_RUN_COUNTER_PKEY ) ) {
		persist_write_int( APP_RUN_COUNTER_PKEY, persist_read_int( APP_RUN_COUNTER_PKEY ) + 1 );
	} else {
		persist_write_int( APP_RUN_COUNTER_PKEY, 1 );
	}
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
  window_set_click_config_provider(s_main_window, click_config_provider);
  
  window_stack_push(s_main_window, true);
	
	increment_app_run_counter();
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}