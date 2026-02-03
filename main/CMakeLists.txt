idf_component_register(SRCS "mini-clock.c" "util.c" "i2c.c" "display.c" "sht45.c"
                       PRIV_REQUIRES esp_driver_i2c
                       INCLUDE_DIRS "./")
