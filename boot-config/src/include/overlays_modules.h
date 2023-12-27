
#if CONFIG_overlays	/* DT-overlays. */
char	*pwr_led="pwr_led",
	*status_led="status_led";
#if CONFIG_overlay_1wire
char	*w1="w1@0\\|w1";
#endif
#if CONFIG_overlay_pps
char	*pps="pps@0\\|pps";
#endif
#if CONFIG_overlay_gpio_keys_user
char	*gpio_keys_user="gpio-keys-user@0\\|gpio-keys-user";
#endif
#if CONFIG_overlay_rotary_encoder
char	*rotary="rotary@0\\|rotary",
	*rotary_button="rotary_button@0\\|rotary_button",
	*rotary_axis="rotary_axis@0\\|rotary_axis";
#endif
#if CONFIG_overlay_i2c4
char	*i2c4="i2c@4\\|i2c4";
#endif
#endif			/* End DT-overlays. */

