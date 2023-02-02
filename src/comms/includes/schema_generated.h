// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_SCHEMA_COMMSSCHEMA_H_
#define FLATBUFFERS_GENERATED_SCHEMA_COMMSSCHEMA_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 23 && FLATBUFFERS_VERSION_MINOR == 1
                  && FLATBUFFERS_VERSION_REVISION == 21,
              "Non-compatible flatbuffers version included");

namespace CommsSchema
{

struct VariableVals;

struct Version;

struct Command;
struct CommandBuilder;

enum CommandID : int8_t
{
    CommandID_OPEN = 0,
    CommandID_CLOSE = 1,
    CommandID_SET = 2,
    CommandID_GET = 3,
    CommandID_RET = 4,
    CommandID_ERR = 5,
    CommandID_RES = 6,
    CommandID_SAVE = 7,
    CommandID_MIN = CommandID_OPEN,
    CommandID_MAX = CommandID_SAVE
};

inline const CommandID (&EnumValuesCommandID())[8]
{
    static const CommandID values[] = {CommandID_OPEN,
                                       CommandID_CLOSE,
                                       CommandID_SET,
                                       CommandID_GET,
                                       CommandID_RET,
                                       CommandID_ERR,
                                       CommandID_RES,
                                       CommandID_SAVE};
    return values;
}

inline const char* const* EnumNamesCommandID()
{
    static const char* const names[9]
        = {"OPEN", "CLOSE", "SET", "GET", "RET", "ERR", "RES", "SAVE", nullptr};
    return names;
}

inline const char* EnumNameCommandID(CommandID e)
{
    if (::flatbuffers::IsOutRange(e, CommandID_OPEN, CommandID_SAVE))
        return "";
    const size_t index = static_cast<size_t>(e);
    return EnumNamesCommandID()[index];
}

enum InverterMode : int8_t
{
    InverterMode_TORQUE = 0,
    InverterMode_SPEED = 1,
    InverterMode_MIN = InverterMode_TORQUE,
    InverterMode_MAX = InverterMode_SPEED
};

inline const InverterMode (&EnumValuesInverterMode())[2]
{
    static const InverterMode values[]
        = {InverterMode_TORQUE, InverterMode_SPEED};
    return values;
}

inline const char* const* EnumNamesInverterMode()
{
    static const char* const names[3] = {"TORQUE", "SPEED", nullptr};
    return names;
}

inline const char* EnumNameInverterMode(InverterMode e)
{
    if (::flatbuffers::IsOutRange(e, InverterMode_TORQUE, InverterMode_SPEED))
        return "";
    const size_t index = static_cast<size_t>(e);
    return EnumNamesInverterMode()[index];
}

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) VariableVals FLATBUFFERS_FINAL_CLASS
{
private:

    uint8_t torque_map_val_[2048];
    int8_t inverter_mode_val_;
    uint8_t disable_torque_requests_val_;
    uint16_t apps_1_adc_min_val_;
    uint16_t apps_1_adc_max_val_;
    uint16_t apps_2_adc_min_val_;
    uint16_t apps_2_adc_max_val_;
    uint16_t bps_adc_min_val_;
    uint16_t bps_adc_max_val_;
    int16_t padding0__;
    uint32_t bps_fully_pressed_threshold_val_;
    uint8_t enable_lapsim_testbench_val_;
    uint8_t lapsim_testbench_laps_val_;
    int16_t padding1__;

public:

    VariableVals()
        : torque_map_val_(), inverter_mode_val_(0),
          disable_torque_requests_val_(0), apps_1_adc_min_val_(0),
          apps_1_adc_max_val_(0), apps_2_adc_min_val_(0),
          apps_2_adc_max_val_(0), bps_adc_min_val_(0), bps_adc_max_val_(0),
          padding0__(0), bps_fully_pressed_threshold_val_(0),
          enable_lapsim_testbench_val_(0), lapsim_testbench_laps_val_(0),
          padding1__(0)
    {
        (void) padding0__;
        (void) padding1__;
    }
    VariableVals(CommsSchema::InverterMode _inverter_mode_val,
                 bool _disable_torque_requests_val,
                 uint16_t _apps_1_adc_min_val,
                 uint16_t _apps_1_adc_max_val,
                 uint16_t _apps_2_adc_min_val,
                 uint16_t _apps_2_adc_max_val,
                 uint16_t _bps_adc_min_val,
                 uint16_t _bps_adc_max_val,
                 uint32_t _bps_fully_pressed_threshold_val,
                 bool _enable_lapsim_testbench_val,
                 uint8_t _lapsim_testbench_laps_val)
        : torque_map_val_(), inverter_mode_val_(::flatbuffers::EndianScalar(
                                 static_cast<int8_t>(_inverter_mode_val))),
          disable_torque_requests_val_(::flatbuffers::EndianScalar(
              static_cast<uint8_t>(_disable_torque_requests_val))),
          apps_1_adc_min_val_(::flatbuffers::EndianScalar(_apps_1_adc_min_val)),
          apps_1_adc_max_val_(::flatbuffers::EndianScalar(_apps_1_adc_max_val)),
          apps_2_adc_min_val_(::flatbuffers::EndianScalar(_apps_2_adc_min_val)),
          apps_2_adc_max_val_(::flatbuffers::EndianScalar(_apps_2_adc_max_val)),
          bps_adc_min_val_(::flatbuffers::EndianScalar(_bps_adc_min_val)),
          bps_adc_max_val_(::flatbuffers::EndianScalar(_bps_adc_max_val)),
          padding0__(0),
          bps_fully_pressed_threshold_val_(
              ::flatbuffers::EndianScalar(_bps_fully_pressed_threshold_val)),
          enable_lapsim_testbench_val_(::flatbuffers::EndianScalar(
              static_cast<uint8_t>(_enable_lapsim_testbench_val))),
          lapsim_testbench_laps_val_(
              ::flatbuffers::EndianScalar(_lapsim_testbench_laps_val)),
          padding1__(0)
    {
        (void) padding0__;
        (void) padding1__;
    }
    VariableVals(::flatbuffers::span<const uint8_t, 2048> _torque_map_val,
                 CommsSchema::InverterMode _inverter_mode_val,
                 bool _disable_torque_requests_val,
                 uint16_t _apps_1_adc_min_val,
                 uint16_t _apps_1_adc_max_val,
                 uint16_t _apps_2_adc_min_val,
                 uint16_t _apps_2_adc_max_val,
                 uint16_t _bps_adc_min_val,
                 uint16_t _bps_adc_max_val,
                 uint32_t _bps_fully_pressed_threshold_val,
                 bool _enable_lapsim_testbench_val,
                 uint8_t _lapsim_testbench_laps_val)
        : inverter_mode_val_(::flatbuffers::EndianScalar(
            static_cast<int8_t>(_inverter_mode_val))),
          disable_torque_requests_val_(::flatbuffers::EndianScalar(
              static_cast<uint8_t>(_disable_torque_requests_val))),
          apps_1_adc_min_val_(::flatbuffers::EndianScalar(_apps_1_adc_min_val)),
          apps_1_adc_max_val_(::flatbuffers::EndianScalar(_apps_1_adc_max_val)),
          apps_2_adc_min_val_(::flatbuffers::EndianScalar(_apps_2_adc_min_val)),
          apps_2_adc_max_val_(::flatbuffers::EndianScalar(_apps_2_adc_max_val)),
          bps_adc_min_val_(::flatbuffers::EndianScalar(_bps_adc_min_val)),
          bps_adc_max_val_(::flatbuffers::EndianScalar(_bps_adc_max_val)),
          padding0__(0),
          bps_fully_pressed_threshold_val_(
              ::flatbuffers::EndianScalar(_bps_fully_pressed_threshold_val)),
          enable_lapsim_testbench_val_(::flatbuffers::EndianScalar(
              static_cast<uint8_t>(_enable_lapsim_testbench_val))),
          lapsim_testbench_laps_val_(
              ::flatbuffers::EndianScalar(_lapsim_testbench_laps_val)),
          padding1__(0)
    {
        ::flatbuffers::CastToArray(torque_map_val_)
            .CopyFromSpan(_torque_map_val);
        (void) padding0__;
        (void) padding1__;
    }
    const ::flatbuffers::Array<uint8_t, 2048>* torque_map_val() const
    {
        return &::flatbuffers::CastToArray(torque_map_val_);
    }
    CommsSchema::InverterMode inverter_mode_val() const
    {
        return static_cast<CommsSchema::InverterMode>(
            ::flatbuffers::EndianScalar(inverter_mode_val_));
    }
    bool disable_torque_requests_val() const
    {
        return ::flatbuffers::EndianScalar(disable_torque_requests_val_) != 0;
    }
    uint16_t apps_1_adc_min_val() const
    {
        return ::flatbuffers::EndianScalar(apps_1_adc_min_val_);
    }
    uint16_t apps_1_adc_max_val() const
    {
        return ::flatbuffers::EndianScalar(apps_1_adc_max_val_);
    }
    uint16_t apps_2_adc_min_val() const
    {
        return ::flatbuffers::EndianScalar(apps_2_adc_min_val_);
    }
    uint16_t apps_2_adc_max_val() const
    {
        return ::flatbuffers::EndianScalar(apps_2_adc_max_val_);
    }
    uint16_t bps_adc_min_val() const
    {
        return ::flatbuffers::EndianScalar(bps_adc_min_val_);
    }
    uint16_t bps_adc_max_val() const
    {
        return ::flatbuffers::EndianScalar(bps_adc_max_val_);
    }
    uint32_t bps_fully_pressed_threshold_val() const
    {
        return ::flatbuffers::EndianScalar(bps_fully_pressed_threshold_val_);
    }
    bool enable_lapsim_testbench_val() const
    {
        return ::flatbuffers::EndianScalar(enable_lapsim_testbench_val_) != 0;
    }
    uint8_t lapsim_testbench_laps_val() const
    {
        return ::flatbuffers::EndianScalar(lapsim_testbench_laps_val_);
    }
};
FLATBUFFERS_STRUCT_END(VariableVals, 2072);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(1) Version FLATBUFFERS_FINAL_CLASS
{
private:

    uint8_t a_;
    uint8_t b_;
    uint8_t c_;

public:

    Version() : a_(0), b_(0), c_(0)
    {
    }
    Version(uint8_t _a, uint8_t _b, uint8_t _c)
        : a_(::flatbuffers::EndianScalar(_a)),
          b_(::flatbuffers::EndianScalar(_b)),
          c_(::flatbuffers::EndianScalar(_c))
    {
    }
    uint8_t a() const
    {
        return ::flatbuffers::EndianScalar(a_);
    }
    uint8_t b() const
    {
        return ::flatbuffers::EndianScalar(b_);
    }
    uint8_t c() const
    {
        return ::flatbuffers::EndianScalar(c_);
    }
};
FLATBUFFERS_STRUCT_END(Version, 3);

struct Command FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table
{
    typedef CommandBuilder Builder;
    enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE
    {
        VT_ID = 4,
        VT_VALS = 6,
        VT_CONFIG_NAME = 8,
        VT_CONFIG_VERSION = 10
    };
    CommsSchema::CommandID id() const
    {
        return static_cast<CommsSchema::CommandID>(GetField<int8_t>(VT_ID, 0));
    }
    const CommsSchema::VariableVals* vals() const
    {
        return GetStruct<const CommsSchema::VariableVals*>(VT_VALS);
    }
    const ::flatbuffers::String* config_name() const
    {
        return GetPointer<const ::flatbuffers::String*>(VT_CONFIG_NAME);
    }
    const CommsSchema::Version* config_version() const
    {
        return GetStruct<const CommsSchema::Version*>(VT_CONFIG_VERSION);
    }
    bool Verify(::flatbuffers::Verifier& verifier) const
    {
        return VerifyTableStart(verifier)
               && VerifyField<int8_t>(verifier, VT_ID, 1)
               && VerifyField<CommsSchema::VariableVals>(verifier, VT_VALS, 4)
               && VerifyOffset(verifier, VT_CONFIG_NAME)
               && verifier.VerifyString(config_name())
               && VerifyField<CommsSchema::Version>(verifier,
                                                    VT_CONFIG_VERSION,
                                                    1)
               && verifier.EndTable();
    }
};

struct CommandBuilder
{
    typedef Command Table;
    ::flatbuffers::FlatBufferBuilder& fbb_;
    ::flatbuffers::uoffset_t start_;
    void add_id(CommsSchema::CommandID id)
    {
        fbb_.AddElement<int8_t>(Command::VT_ID, static_cast<int8_t>(id), 0);
    }
    void add_vals(const CommsSchema::VariableVals* vals)
    {
        fbb_.AddStruct(Command::VT_VALS, vals);
    }
    void
    add_config_name(::flatbuffers::Offset<::flatbuffers::String> config_name)
    {
        fbb_.AddOffset(Command::VT_CONFIG_NAME, config_name);
    }
    void add_config_version(const CommsSchema::Version* config_version)
    {
        fbb_.AddStruct(Command::VT_CONFIG_VERSION, config_version);
    }
    explicit CommandBuilder(::flatbuffers::FlatBufferBuilder& _fbb) : fbb_(_fbb)
    {
        start_ = fbb_.StartTable();
    }
    ::flatbuffers::Offset<Command> Finish()
    {
        const auto end = fbb_.EndTable(start_);
        auto o = ::flatbuffers::Offset<Command>(end);
        return o;
    }
};

inline ::flatbuffers::Offset<Command>
CreateCommand(::flatbuffers::FlatBufferBuilder& _fbb,
              CommsSchema::CommandID id = CommsSchema::CommandID_OPEN,
              const CommsSchema::VariableVals* vals = nullptr,
              ::flatbuffers::Offset<::flatbuffers::String> config_name = 0,
              const CommsSchema::Version* config_version = nullptr)
{
    CommandBuilder builder_(_fbb);
    builder_.add_config_version(config_version);
    builder_.add_config_name(config_name);
    builder_.add_vals(vals);
    builder_.add_id(id);
    return builder_.Finish();
}

inline ::flatbuffers::Offset<Command>
CreateCommandDirect(::flatbuffers::FlatBufferBuilder& _fbb,
                    CommsSchema::CommandID id = CommsSchema::CommandID_OPEN,
                    const CommsSchema::VariableVals* vals = nullptr,
                    const char* config_name = nullptr,
                    const CommsSchema::Version* config_version = nullptr)
{
    auto config_name__ = config_name ? _fbb.CreateString(config_name) : 0;
    return CommsSchema::CreateCommand(_fbb,
                                      id,
                                      vals,
                                      config_name__,
                                      config_version);
}

} // namespace CommsSchema

#endif // FLATBUFFERS_GENERATED_SCHEMA_COMMSSCHEMA_H_