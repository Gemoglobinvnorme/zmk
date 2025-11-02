#define DT_DRV_COMPAT zmk_behavior_language_choose
#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/event_manager.h>
#include <zmk/keymap.h>
#include <zephyr/logging/log.h>
#include <dt-bindings/zmk/language.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

int global_language = LANG_EN;

static int behavior_language_choose_init(const struct device *dev) {
    global_language = LANG_EN;

    return 0;
};

static int lch_press(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event)
{
    uint32_t language = binding->param1;
    uint32_t keycode = binding->param2;
    LOG_DBG("DM press: keycode=%d", keycode);

    global_language = language;

    struct zmk_behavior_binding kp_binding = {
        .behavior_dev = DEVICE_DT_NAME(DT_NODELABEL(kp)),
        .param1 = keycode,
    };

    return zmk_behavior_invoke_binding(&kp_binding, event, true);
}

static int lch_release(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event)
{
    uint32_t keycode = binding->param2;
    LOG_DBG("DM release: keycode=%d", keycode);

    struct zmk_behavior_binding kp_binding = {
        .behavior_dev = DEVICE_DT_NAME(DT_NODELABEL(kp)),
        .param1 = keycode,
    };

    return zmk_behavior_invoke_binding(&kp_binding, event, false);
}

static const struct behavior_driver_api lch_driver_api = {
    .binding_pressed = lch_press,
    .binding_released = lch_release,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL,
                        NULL, NULL,
                        APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &lch_driver_api);
