/*
 * EMS-ESP - https://github.com/proddy/EMS-ESP
 * Copyright 2020  Paul Derbyshire
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EMSESP_MIXER_H
#define EMSESP_MIXER_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include <uuid/log.h>

#include "emsdevice.h"
#include "emsesp.h"
#include "telegram.h"
#include "helpers.h"
#include "mqtt.h"

namespace emsesp {

class Mixer : public EMSdevice {
  public:
    Mixer(uint8_t device_type, uint8_t device_id, uint8_t product_id, const std::string & version, const std::string & name, uint8_t flags, uint8_t brand);

    virtual void publish_values(JsonObject & json, bool force);
    virtual bool export_values(JsonObject & json, int8_t id = -1);
    virtual void device_info_web(JsonArray & root, uint8_t & part);
    virtual bool updated_values();

  private:
    static uuid::log::Logger logger_;

    bool export_values_format(uint8_t mqtt_format, JsonObject & doc);
    void register_mqtt_ha_config();

    void process_MMPLUSStatusMessage_HC(std::shared_ptr<const Telegram> telegram);
    void process_MMPLUSStatusMessage_WWC(std::shared_ptr<const Telegram> telegram);
    void process_IPMStatusMessage(std::shared_ptr<const Telegram> telegram);
    void process_MMStatusMessage(std::shared_ptr<const Telegram> telegram);
    void process_MMConfigMessage(std::shared_ptr<const Telegram> telegram);
    void process_MMSetMessage(std::shared_ptr<const Telegram> telegram);

    enum class Type {
        NONE,
        HC, // heating circuit
        WWC // warm water circuit
    };

    Type type() const {
        return type_;
    }

    void type(Type new_type) {
        type_ = new_type;
    }

  private:
    uint16_t hc_          = EMS_VALUE_USHORT_NOTSET;
    uint16_t flowTemp_    = EMS_VALUE_USHORT_NOTSET;
    uint8_t  pumpStatus_  = EMS_VALUE_UINT_NOTSET;
    int8_t   status_      = EMS_VALUE_INT_NOTSET;
    uint8_t  flowSetTemp_ = EMS_VALUE_UINT_NOTSET;

    Type type_ = Type::NONE;

    bool changed_        = false;
    bool mqtt_ha_config_ = false; // for HA MQTT Discovery
};

} // namespace emsesp

#endif
