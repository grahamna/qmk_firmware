#include QMK_KEYBOARD_H


enum {
   MJ = SAFE_RANGE,
};

#if defined(DEFERRED_EXEC_ENABLE) && defined(MOUSE_ENABLE)
   #define TICK 50000
   #define BASE 20000
   #define MULT 19
   static bool keeping_timestamps = false;
   static deferred_token mj_token = INVALID_DEFERRED_TOKEN;

   // Generates a pseudorandom value in 0-255.
   static uint8_t simple_rand(void) {
      static uint16_t random = 1;
      random *= UINT16_C(36563);
      return (uint8_t)(random >> 8);
   };

   uint32_t mj_callback(uint32_t trigger_time, void *cb_arg) {
      uint16_t rand = simple_rand();
      uint8_t dir = rand % 4;
      switch(dir) {
         case 0:
            tap_code16(QK_MOUSE_CURSOR_UP);
            break;
         case 1:
            tap_code16(QK_MOUSE_CURSOR_DOWN);
            break;
         case 2:
            tap_code16(QK_MOUSE_CURSOR_LEFT);
            break;
         case 3:
            tap_code16(QK_MOUSE_CURSOR_RIGHT);
            break;
      }
      return keeping_timestamps ? (BASE + (rand * MULT)) : 0;
   };

   bool process_record_user(uint16_t keycode, keyrecord_t *record) {
      if (mj_token) {
         extend_deferred_exec(mj_token, TICK);
      }
      switch(keycode) {
         case MJ:
            if(record->event.pressed) {
               if (!keeping_timestamps) {
                  keeping_timestamps = true;
                  rgb_matrix_toggle();
                  mj_token = defer_exec(TICK, mj_callback, NULL);
                  return false; break;
               }
               if (keeping_timestamps) {
                  keeping_timestamps = false;
                  cancel_deferred_exec(mj_token);
                  rgb_matrix_toggle();
                  return false; break;
               }
            }
            else {}
      }
      return true;
   };
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT(
//┌────────┬────────┬────────┬────────┬────────┬────────┐                   ┌────────┬────────┬────────┬────────┬────────┬────────┐
   QK_GESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
//├────────┼────────┼────────┼────────┼────────┼────────┤                   ├────────┼────────┼────────┼────────┼────────┼────────┤
	KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQL,
//├────────┼────────┼────────┼────────┼────────┼────────┤                   ├────────┼────────┼────────┼────────┼────────┼────────┤
LSFT_T(KC_BSPC), KC_A, KC_S,   KC_D,    KC_F,    KC_G,                        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
//├────────┼────────┼────────┼────────┼────────┼────────┼────────┐ ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
LCTL_T(KC_LGUI), KC_Z, KC_X,   KC_C,    KC_V,    KC_B,   KC_HOME,    KC_END,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, SC_SENT,
//└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘ └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
							   LALT_T(KC_DEL), KC_LSFT, LT(1,KC_ENT),     LT(2,KC_SPC), RSFT_T(KC_SPC), RALT_T(KC_BSPC)
//                               └────────┴────────┴────────┘          └────────┴────────┴────────┘
	),

	[1] = LAYOUT(
//┌────────┬────────┬────────┬────────┬────────┬────────┐                   ┌────────┬────────┬────────┬────────┬────────┬────────┐
   KC_GRV,  KC_F1,   KC_F2,   KC_F3,  KC_F4,    KC_F5,                       KC_F6,    KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F12,
//├────────┼────────┼────────┼────────┼────────┼────────┤                   ├────────┼────────┼────────┼────────┼────────┼────────┤
   KC_TRNS, KC_LCBR, KC_RCBR, KC_PLUS, KC_AMPR, KC_HASH,                     KC_VOLU,  KC_7,    KC_8,    KC_9,   KC_SLSH,  KC_F11,
//├────────┼────────┼────────┼────────┼────────┼────────┤                   ├────────┼────────┼────────┼────────┼────────┼────────┤
   KC_TRNS, KC_LPRN, KC_RPRN, KC_MINS, KC_EXLM, KC_QUES,                     KC_VOLD,  KC_4,    KC_5,    KC_6,   KC_MINS, SC_SENT,
//├────────┼────────┼────────┼────────┼────────┼────────┼────────┐ ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
   KC_LSFT, KC_LBRC, KC_RBRC, KC_UNDS, KC_PIPE, KC_BSLS, KC_PSCR,   KC_CAPS,  KC_MUTE, KC_1,    KC_2,    KC_3,   KC_EQL,  KC_TRNS,
//└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘ └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
					               KC_TRNS, LSFT_T(KC_BSPC), KC_TRNS,     RSFT_T(KC_SPC), KC_0,  KC_DOT
//                               └────────┴────────┴────────┘          └────────┴────────┴────────┘
	),

	[2] = LAYOUT(
//┌────────┬────────┬────────┬────────┬────────┬────────┐                   ┌────────┬────────┬────────┬────────┬────────┬────────┐
	QK_GESC, KC_F1,   KC_F2,   KC_F3,   KC_F4,    KC_F5,                      KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F12,
//├────────┼────────┼────────┼────────┼────────┼────────┤                   ├────────┼────────┼────────┼────────┼────────┼────────┤
   RGB_VAI, RGB_TOG, KC_WBAK, KC_MS_U, KC_WFWD, KC_VOLU,                     KC_F5,   KC_F10,   KC_UP,  KC_F11,  KC_PSCR, KC_TRNS,
//├────────┼────────┼────────┼────────┼────────┼────────┤                   ├────────┼────────┼────────┼────────┼────────┼────────┤
   RGB_VAD, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_VOLD,                     KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT,  KC_END, KC_TRNS,
//├────────┼────────┼────────┼────────┼────────┼────────┤                   ├────────┼────────┼────────┼────────┼────────┼────────┤
   KC_LSFT, KC_TRNS, KC_BTN2, KC_BTN3, KC_BTN1, KC_MUTE,  TG(3),     TG(4),  KC_ACL0, KC_PGDN, KC_PGUP, KC_TRNS, KC_TRNS, KC_TRNS,
//└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘ └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                      LALT_T(KC_DEL), LSFT_T(KC_BSPC), LCTL_T(KC_SPC),  KC_TRNS, KC_TRNS, KC_TRNS
//                               └────────┴────────┴────────┘          └────────┴────────┴────────┘
	),

	[3] = LAYOUT(
//┌────────┬────────┬────────┬────────┬────────┬────────┐                   ┌────────┬────────┬────────┬────────┬────────┬────────┐
   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
//├────────┼────────┼────────┼────────┼────────┼────────┤                   ├────────┼────────┼────────┼────────┼────────┼────────┤
   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                     RGB_SAI, RGB_SPI, RGB_VAI, RGB_HUI, RGB_MOD, KC_TRNS, 
//├────────┼────────┼────────┼────────┼────────┼────────┤                   ├────────┼────────┼────────┼────────┼────────┼────────┤
   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                     RGB_SAD, RGB_SPD, RGB_VAD, RGB_HUD, RGB_RMOD, KC_TRNS, 
//├────────┼────────┼────────┼────────┼────────┼────────┤                   ├────────┼────────┼────────┼────────┼────────┼────────┤
   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  TG(3),      MJ,    RGB_TOG, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
//└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘ └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
								         KC_TRNS, KC_LCTL, KC_SPC,              KC_TRNS, KC_TRNS, KC_TRNS
//                               └────────┴────────┴────────┘          └────────┴────────┴────────┘
	),

   [4] = LAYOUT(
//┌────────┬────────┬────────┬────────┬────────┬────────┐                   ┌────────┬────────┬────────┬────────┬────────┬────────┐
   QK_GESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
//├────────┼────────┼────────┼────────┼────────┼────────┤                   ├────────┼────────┼────────┼────────┼────────┼────────┤
	KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQL,
//├────────┼────────┼────────┼────────┼────────┼────────┤                   ├────────┼────────┼────────┼────────┼────────┼────────┤
LSFT_T(KC_BSPC), KC_A, KC_S,   KC_D,    KC_F,    KC_G,                        KC_H,    KC_J,   KC_K,     KC_L,    KC_SCLN, KC_QUOT,
//├────────┼────────┼────────┼────────┼────────┼────────┼────────┐ ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
LCTL_T(KC_ENT), KC_Z, KC_X,   KC_C,    KC_V,    KC_B,   KC_HOME,    TG(4),   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, SC_SENT,
//└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘ └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
							   LALT_T(KC_DEL), KC_LSFT, LT(1,KC_SPC),     LT(2,KC_SPC), RSFT_T(KC_SPC), RALT_T(KC_BSPC)
//                               └────────┴────────┴────────┘          └────────┴────────┴────────┘
	)
}; 

#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {};
#endif




