// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_RPC_IA20_NET_ENGINE_RAFT_FB_H_
#define FLATBUFFERS_GENERATED_RPC_IA20_NET_ENGINE_RAFT_FB_H_

#include "flatbuffers/flatbuffers.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace FB {

struct Header;

struct LogEntryId;

struct VoteRequest;
struct VoteRequestBuilder;

struct VoteResponse;
struct VoteResponseBuilder;

struct AppendLogRequest;
struct AppendLogRequestBuilder;

struct AppendLogResponse;
struct AppendLogResponseBuilder;

struct Message;
struct MessageBuilder;

inline const flatbuffers::TypeTable *HeaderTypeTable();

inline const flatbuffers::TypeTable *LogEntryIdTypeTable();

inline const flatbuffers::TypeTable *VoteRequestTypeTable();

inline const flatbuffers::TypeTable *VoteResponseTypeTable();

inline const flatbuffers::TypeTable *AppendLogRequestTypeTable();

inline const flatbuffers::TypeTable *AppendLogResponseTypeTable();

inline const flatbuffers::TypeTable *MessageTypeTable();

enum Action {
  Action_NONE = 0,
  Action_VoteRequest = 1,
  Action_VoteResponse = 2,
  Action_AppendLogRequest = 3,
  Action_AppendLogResponse = 4,
  Action_MIN = Action_NONE,
  Action_MAX = Action_AppendLogResponse
};

inline const Action (&EnumValuesAction())[5] {
  static const Action values[] = {
    Action_NONE,
    Action_VoteRequest,
    Action_VoteResponse,
    Action_AppendLogRequest,
    Action_AppendLogResponse
  };
  return values;
}

inline const char * const *EnumNamesAction() {
  static const char * const names[6] = {
    "NONE",
    "VoteRequest",
    "VoteResponse",
    "AppendLogRequest",
    "AppendLogResponse",
    nullptr
  };
  return names;
}

inline const char *EnumNameAction(Action e) {
  if (flatbuffers::IsOutRange(e, Action_NONE, Action_AppendLogResponse)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesAction()[index];
}

template<typename T> struct ActionTraits {
  static const Action enum_value = Action_NONE;
};

template<> struct ActionTraits<IA20::Net::Engine::Raft::FB::VoteRequest> {
  static const Action enum_value = Action_VoteRequest;
};

template<> struct ActionTraits<IA20::Net::Engine::Raft::FB::VoteResponse> {
  static const Action enum_value = Action_VoteResponse;
};

template<> struct ActionTraits<IA20::Net::Engine::Raft::FB::AppendLogRequest> {
  static const Action enum_value = Action_AppendLogRequest;
};

template<> struct ActionTraits<IA20::Net::Engine::Raft::FB::AppendLogResponse> {
  static const Action enum_value = Action_AppendLogResponse;
};

bool VerifyAction(flatbuffers::Verifier &verifier, const void *obj, Action type);
bool VerifyActionVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(2) Header FLATBUFFERS_FINAL_CLASS {
 private:
  uint16_t dstServerId_;
  uint16_t srcServerId_;

 public:
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return HeaderTypeTable();
  }
  Header() {
    memset(static_cast<void *>(this), 0, sizeof(Header));
  }
  Header(uint16_t _dstServerId, uint16_t _srcServerId)
      : dstServerId_(flatbuffers::EndianScalar(_dstServerId)),
        srcServerId_(flatbuffers::EndianScalar(_srcServerId)) {
  }
  uint16_t dstServerId() const {
    return flatbuffers::EndianScalar(dstServerId_);
  }
  uint16_t srcServerId() const {
    return flatbuffers::EndianScalar(srcServerId_);
  }
};
FLATBUFFERS_STRUCT_END(Header, 4);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) LogEntryId FLATBUFFERS_FINAL_CLASS {
 private:
  uint16_t term_;
  int16_t padding0__;
  uint32_t index_;

 public:
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return LogEntryIdTypeTable();
  }
  LogEntryId() {
    memset(static_cast<void *>(this), 0, sizeof(LogEntryId));
  }
  LogEntryId(uint16_t _term, uint32_t _index)
      : term_(flatbuffers::EndianScalar(_term)),
        padding0__(0),
        index_(flatbuffers::EndianScalar(_index)) {
    (void)padding0__;
  }
  uint16_t term() const {
    return flatbuffers::EndianScalar(term_);
  }
  uint32_t index() const {
    return flatbuffers::EndianScalar(index_);
  }
};
FLATBUFFERS_STRUCT_END(LogEntryId, 8);

struct VoteRequest FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef VoteRequestBuilder Builder;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return VoteRequestTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TERM = 4,
    VT_CANDIDATE = 6,
    VT_LASTLOGENTRY = 8
  };
  uint16_t term() const {
    return GetField<uint16_t>(VT_TERM, 0);
  }
  uint16_t candidate() const {
    return GetField<uint16_t>(VT_CANDIDATE, 0);
  }
  const IA20::Net::Engine::Raft::FB::LogEntryId *lastLogEntry() const {
    return GetStruct<const IA20::Net::Engine::Raft::FB::LogEntryId *>(VT_LASTLOGENTRY);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_TERM) &&
           VerifyField<uint16_t>(verifier, VT_CANDIDATE) &&
           VerifyField<IA20::Net::Engine::Raft::FB::LogEntryId>(verifier, VT_LASTLOGENTRY) &&
           verifier.EndTable();
  }
};

struct VoteRequestBuilder {
  typedef VoteRequest Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_term(uint16_t term) {
    fbb_.AddElement<uint16_t>(VoteRequest::VT_TERM, term, 0);
  }
  void add_candidate(uint16_t candidate) {
    fbb_.AddElement<uint16_t>(VoteRequest::VT_CANDIDATE, candidate, 0);
  }
  void add_lastLogEntry(const IA20::Net::Engine::Raft::FB::LogEntryId *lastLogEntry) {
    fbb_.AddStruct(VoteRequest::VT_LASTLOGENTRY, lastLogEntry);
  }
  explicit VoteRequestBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  VoteRequestBuilder &operator=(const VoteRequestBuilder &);
  flatbuffers::Offset<VoteRequest> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<VoteRequest>(end);
    return o;
  }
};

inline flatbuffers::Offset<VoteRequest> CreateVoteRequest(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t term = 0,
    uint16_t candidate = 0,
    const IA20::Net::Engine::Raft::FB::LogEntryId *lastLogEntry = 0) {
  VoteRequestBuilder builder_(_fbb);
  builder_.add_lastLogEntry(lastLogEntry);
  builder_.add_candidate(candidate);
  builder_.add_term(term);
  return builder_.Finish();
}

struct VoteResponse FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef VoteResponseBuilder Builder;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return VoteResponseTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TERM = 4,
    VT_CANDIDATE = 6,
    VT_GRANTED = 8
  };
  uint16_t term() const {
    return GetField<uint16_t>(VT_TERM, 0);
  }
  uint16_t candidate() const {
    return GetField<uint16_t>(VT_CANDIDATE, 0);
  }
  bool granted() const {
    return GetField<uint8_t>(VT_GRANTED, 0) != 0;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_TERM) &&
           VerifyField<uint16_t>(verifier, VT_CANDIDATE) &&
           VerifyField<uint8_t>(verifier, VT_GRANTED) &&
           verifier.EndTable();
  }
};

struct VoteResponseBuilder {
  typedef VoteResponse Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_term(uint16_t term) {
    fbb_.AddElement<uint16_t>(VoteResponse::VT_TERM, term, 0);
  }
  void add_candidate(uint16_t candidate) {
    fbb_.AddElement<uint16_t>(VoteResponse::VT_CANDIDATE, candidate, 0);
  }
  void add_granted(bool granted) {
    fbb_.AddElement<uint8_t>(VoteResponse::VT_GRANTED, static_cast<uint8_t>(granted), 0);
  }
  explicit VoteResponseBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  VoteResponseBuilder &operator=(const VoteResponseBuilder &);
  flatbuffers::Offset<VoteResponse> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<VoteResponse>(end);
    return o;
  }
};

inline flatbuffers::Offset<VoteResponse> CreateVoteResponse(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t term = 0,
    uint16_t candidate = 0,
    bool granted = false) {
  VoteResponseBuilder builder_(_fbb);
  builder_.add_candidate(candidate);
  builder_.add_term(term);
  builder_.add_granted(granted);
  return builder_.Finish();
}

struct AppendLogRequest FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef AppendLogRequestBuilder Builder;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return AppendLogRequestTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_LEADER = 4,
    VT_MATCHLOGENTRY = 6,
    VT_DATALOGENTRY = 8,
    VT_LEADERCOMMITENTRY = 10,
    VT_DATA = 12
  };
  uint16_t leader() const {
    return GetField<uint16_t>(VT_LEADER, 0);
  }
  const IA20::Net::Engine::Raft::FB::LogEntryId *matchLogEntry() const {
    return GetStruct<const IA20::Net::Engine::Raft::FB::LogEntryId *>(VT_MATCHLOGENTRY);
  }
  const IA20::Net::Engine::Raft::FB::LogEntryId *dataLogEntry() const {
    return GetStruct<const IA20::Net::Engine::Raft::FB::LogEntryId *>(VT_DATALOGENTRY);
  }
  const IA20::Net::Engine::Raft::FB::LogEntryId *leaderCommitEntry() const {
    return GetStruct<const IA20::Net::Engine::Raft::FB::LogEntryId *>(VT_LEADERCOMMITENTRY);
  }
  const flatbuffers::Vector<uint8_t> *data() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_LEADER) &&
           VerifyField<IA20::Net::Engine::Raft::FB::LogEntryId>(verifier, VT_MATCHLOGENTRY) &&
           VerifyField<IA20::Net::Engine::Raft::FB::LogEntryId>(verifier, VT_DATALOGENTRY) &&
           VerifyField<IA20::Net::Engine::Raft::FB::LogEntryId>(verifier, VT_LEADERCOMMITENTRY) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.VerifyVector(data()) &&
           verifier.EndTable();
  }
};

struct AppendLogRequestBuilder {
  typedef AppendLogRequest Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_leader(uint16_t leader) {
    fbb_.AddElement<uint16_t>(AppendLogRequest::VT_LEADER, leader, 0);
  }
  void add_matchLogEntry(const IA20::Net::Engine::Raft::FB::LogEntryId *matchLogEntry) {
    fbb_.AddStruct(AppendLogRequest::VT_MATCHLOGENTRY, matchLogEntry);
  }
  void add_dataLogEntry(const IA20::Net::Engine::Raft::FB::LogEntryId *dataLogEntry) {
    fbb_.AddStruct(AppendLogRequest::VT_DATALOGENTRY, dataLogEntry);
  }
  void add_leaderCommitEntry(const IA20::Net::Engine::Raft::FB::LogEntryId *leaderCommitEntry) {
    fbb_.AddStruct(AppendLogRequest::VT_LEADERCOMMITENTRY, leaderCommitEntry);
  }
  void add_data(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data) {
    fbb_.AddOffset(AppendLogRequest::VT_DATA, data);
  }
  explicit AppendLogRequestBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  AppendLogRequestBuilder &operator=(const AppendLogRequestBuilder &);
  flatbuffers::Offset<AppendLogRequest> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<AppendLogRequest>(end);
    return o;
  }
};

inline flatbuffers::Offset<AppendLogRequest> CreateAppendLogRequest(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t leader = 0,
    const IA20::Net::Engine::Raft::FB::LogEntryId *matchLogEntry = 0,
    const IA20::Net::Engine::Raft::FB::LogEntryId *dataLogEntry = 0,
    const IA20::Net::Engine::Raft::FB::LogEntryId *leaderCommitEntry = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data = 0) {
  AppendLogRequestBuilder builder_(_fbb);
  builder_.add_data(data);
  builder_.add_leaderCommitEntry(leaderCommitEntry);
  builder_.add_dataLogEntry(dataLogEntry);
  builder_.add_matchLogEntry(matchLogEntry);
  builder_.add_leader(leader);
  return builder_.Finish();
}

inline flatbuffers::Offset<AppendLogRequest> CreateAppendLogRequestDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t leader = 0,
    const IA20::Net::Engine::Raft::FB::LogEntryId *matchLogEntry = 0,
    const IA20::Net::Engine::Raft::FB::LogEntryId *dataLogEntry = 0,
    const IA20::Net::Engine::Raft::FB::LogEntryId *leaderCommitEntry = 0,
    const std::vector<uint8_t> *data = nullptr) {
  auto data__ = data ? _fbb.CreateVector<uint8_t>(*data) : 0;
  return IA20::Net::Engine::Raft::FB::CreateAppendLogRequest(
      _fbb,
      leader,
      matchLogEntry,
      dataLogEntry,
      leaderCommitEntry,
      data__);
}

struct AppendLogResponse FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef AppendLogResponseBuilder Builder;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return AppendLogResponseTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_MATCHLOGENTRY = 4,
    VT_DATALOGENTRY = 6,
    VT_SUCCESS = 8,
    VT_RESEND = 10
  };
  const IA20::Net::Engine::Raft::FB::LogEntryId *matchLogEntry() const {
    return GetStruct<const IA20::Net::Engine::Raft::FB::LogEntryId *>(VT_MATCHLOGENTRY);
  }
  const IA20::Net::Engine::Raft::FB::LogEntryId *dataLogEntry() const {
    return GetStruct<const IA20::Net::Engine::Raft::FB::LogEntryId *>(VT_DATALOGENTRY);
  }
  bool success() const {
    return GetField<uint8_t>(VT_SUCCESS, 0) != 0;
  }
  bool resend() const {
    return GetField<uint8_t>(VT_RESEND, 0) != 0;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<IA20::Net::Engine::Raft::FB::LogEntryId>(verifier, VT_MATCHLOGENTRY) &&
           VerifyField<IA20::Net::Engine::Raft::FB::LogEntryId>(verifier, VT_DATALOGENTRY) &&
           VerifyField<uint8_t>(verifier, VT_SUCCESS) &&
           VerifyField<uint8_t>(verifier, VT_RESEND) &&
           verifier.EndTable();
  }
};

struct AppendLogResponseBuilder {
  typedef AppendLogResponse Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_matchLogEntry(const IA20::Net::Engine::Raft::FB::LogEntryId *matchLogEntry) {
    fbb_.AddStruct(AppendLogResponse::VT_MATCHLOGENTRY, matchLogEntry);
  }
  void add_dataLogEntry(const IA20::Net::Engine::Raft::FB::LogEntryId *dataLogEntry) {
    fbb_.AddStruct(AppendLogResponse::VT_DATALOGENTRY, dataLogEntry);
  }
  void add_success(bool success) {
    fbb_.AddElement<uint8_t>(AppendLogResponse::VT_SUCCESS, static_cast<uint8_t>(success), 0);
  }
  void add_resend(bool resend) {
    fbb_.AddElement<uint8_t>(AppendLogResponse::VT_RESEND, static_cast<uint8_t>(resend), 0);
  }
  explicit AppendLogResponseBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  AppendLogResponseBuilder &operator=(const AppendLogResponseBuilder &);
  flatbuffers::Offset<AppendLogResponse> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<AppendLogResponse>(end);
    return o;
  }
};

inline flatbuffers::Offset<AppendLogResponse> CreateAppendLogResponse(
    flatbuffers::FlatBufferBuilder &_fbb,
    const IA20::Net::Engine::Raft::FB::LogEntryId *matchLogEntry = 0,
    const IA20::Net::Engine::Raft::FB::LogEntryId *dataLogEntry = 0,
    bool success = false,
    bool resend = false) {
  AppendLogResponseBuilder builder_(_fbb);
  builder_.add_dataLogEntry(dataLogEntry);
  builder_.add_matchLogEntry(matchLogEntry);
  builder_.add_resend(resend);
  builder_.add_success(success);
  return builder_.Finish();
}

struct Message FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef MessageBuilder Builder;
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return MessageTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_HEADER = 4,
    VT_ACTION_TYPE = 6,
    VT_ACTION = 8
  };
  const IA20::Net::Engine::Raft::FB::Header *header() const {
    return GetStruct<const IA20::Net::Engine::Raft::FB::Header *>(VT_HEADER);
  }
  IA20::Net::Engine::Raft::FB::Action action_type() const {
    return static_cast<IA20::Net::Engine::Raft::FB::Action>(GetField<uint8_t>(VT_ACTION_TYPE, 0));
  }
  const void *action() const {
    return GetPointer<const void *>(VT_ACTION);
  }
  template<typename T> const T *action_as() const;
  const IA20::Net::Engine::Raft::FB::VoteRequest *action_as_VoteRequest() const {
    return action_type() == IA20::Net::Engine::Raft::FB::Action_VoteRequest ? static_cast<const IA20::Net::Engine::Raft::FB::VoteRequest *>(action()) : nullptr;
  }
  const IA20::Net::Engine::Raft::FB::VoteResponse *action_as_VoteResponse() const {
    return action_type() == IA20::Net::Engine::Raft::FB::Action_VoteResponse ? static_cast<const IA20::Net::Engine::Raft::FB::VoteResponse *>(action()) : nullptr;
  }
  const IA20::Net::Engine::Raft::FB::AppendLogRequest *action_as_AppendLogRequest() const {
    return action_type() == IA20::Net::Engine::Raft::FB::Action_AppendLogRequest ? static_cast<const IA20::Net::Engine::Raft::FB::AppendLogRequest *>(action()) : nullptr;
  }
  const IA20::Net::Engine::Raft::FB::AppendLogResponse *action_as_AppendLogResponse() const {
    return action_type() == IA20::Net::Engine::Raft::FB::Action_AppendLogResponse ? static_cast<const IA20::Net::Engine::Raft::FB::AppendLogResponse *>(action()) : nullptr;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<IA20::Net::Engine::Raft::FB::Header>(verifier, VT_HEADER) &&
           VerifyField<uint8_t>(verifier, VT_ACTION_TYPE) &&
           VerifyOffset(verifier, VT_ACTION) &&
           VerifyAction(verifier, action(), action_type()) &&
           verifier.EndTable();
  }
};

template<> inline const IA20::Net::Engine::Raft::FB::VoteRequest *Message::action_as<IA20::Net::Engine::Raft::FB::VoteRequest>() const {
  return action_as_VoteRequest();
}

template<> inline const IA20::Net::Engine::Raft::FB::VoteResponse *Message::action_as<IA20::Net::Engine::Raft::FB::VoteResponse>() const {
  return action_as_VoteResponse();
}

template<> inline const IA20::Net::Engine::Raft::FB::AppendLogRequest *Message::action_as<IA20::Net::Engine::Raft::FB::AppendLogRequest>() const {
  return action_as_AppendLogRequest();
}

template<> inline const IA20::Net::Engine::Raft::FB::AppendLogResponse *Message::action_as<IA20::Net::Engine::Raft::FB::AppendLogResponse>() const {
  return action_as_AppendLogResponse();
}

struct MessageBuilder {
  typedef Message Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_header(const IA20::Net::Engine::Raft::FB::Header *header) {
    fbb_.AddStruct(Message::VT_HEADER, header);
  }
  void add_action_type(IA20::Net::Engine::Raft::FB::Action action_type) {
    fbb_.AddElement<uint8_t>(Message::VT_ACTION_TYPE, static_cast<uint8_t>(action_type), 0);
  }
  void add_action(flatbuffers::Offset<void> action) {
    fbb_.AddOffset(Message::VT_ACTION, action);
  }
  explicit MessageBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MessageBuilder &operator=(const MessageBuilder &);
  flatbuffers::Offset<Message> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Message>(end);
    return o;
  }
};

inline flatbuffers::Offset<Message> CreateMessage(
    flatbuffers::FlatBufferBuilder &_fbb,
    const IA20::Net::Engine::Raft::FB::Header *header = 0,
    IA20::Net::Engine::Raft::FB::Action action_type = IA20::Net::Engine::Raft::FB::Action_NONE,
    flatbuffers::Offset<void> action = 0) {
  MessageBuilder builder_(_fbb);
  builder_.add_action(action);
  builder_.add_header(header);
  builder_.add_action_type(action_type);
  return builder_.Finish();
}

inline bool VerifyAction(flatbuffers::Verifier &verifier, const void *obj, Action type) {
  switch (type) {
    case Action_NONE: {
      return true;
    }
    case Action_VoteRequest: {
      auto ptr = reinterpret_cast<const IA20::Net::Engine::Raft::FB::VoteRequest *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case Action_VoteResponse: {
      auto ptr = reinterpret_cast<const IA20::Net::Engine::Raft::FB::VoteResponse *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case Action_AppendLogRequest: {
      auto ptr = reinterpret_cast<const IA20::Net::Engine::Raft::FB::AppendLogRequest *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case Action_AppendLogResponse: {
      auto ptr = reinterpret_cast<const IA20::Net::Engine::Raft::FB::AppendLogResponse *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return true;
  }
}

inline bool VerifyActionVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyAction(
        verifier,  values->Get(i), types->GetEnum<Action>(i))) {
      return false;
    }
  }
  return true;
}

inline const flatbuffers::TypeTable *ActionTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_SEQUENCE, 0, -1 },
    { flatbuffers::ET_SEQUENCE, 0, 0 },
    { flatbuffers::ET_SEQUENCE, 0, 1 },
    { flatbuffers::ET_SEQUENCE, 0, 2 },
    { flatbuffers::ET_SEQUENCE, 0, 3 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    IA20::Net::Engine::Raft::FB::VoteRequestTypeTable,
    IA20::Net::Engine::Raft::FB::VoteResponseTypeTable,
    IA20::Net::Engine::Raft::FB::AppendLogRequestTypeTable,
    IA20::Net::Engine::Raft::FB::AppendLogResponseTypeTable
  };
  static const char * const names[] = {
    "NONE",
    "VoteRequest",
    "VoteResponse",
    "AppendLogRequest",
    "AppendLogResponse"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_UNION, 5, type_codes, type_refs, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *HeaderTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_USHORT, 0, -1 },
    { flatbuffers::ET_USHORT, 0, -1 }
  };
  static const int64_t values[] = { 0, 2, 4 };
  static const char * const names[] = {
    "dstServerId",
    "srcServerId"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_STRUCT, 2, type_codes, nullptr, values, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *LogEntryIdTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_USHORT, 0, -1 },
    { flatbuffers::ET_UINT, 0, -1 }
  };
  static const int64_t values[] = { 0, 4, 8 };
  static const char * const names[] = {
    "term",
    "index"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_STRUCT, 2, type_codes, nullptr, values, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *VoteRequestTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_USHORT, 0, -1 },
    { flatbuffers::ET_USHORT, 0, -1 },
    { flatbuffers::ET_SEQUENCE, 0, 0 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    IA20::Net::Engine::Raft::FB::LogEntryIdTypeTable
  };
  static const char * const names[] = {
    "term",
    "candidate",
    "lastLogEntry"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 3, type_codes, type_refs, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *VoteResponseTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_USHORT, 0, -1 },
    { flatbuffers::ET_USHORT, 0, -1 },
    { flatbuffers::ET_BOOL, 0, -1 }
  };
  static const char * const names[] = {
    "term",
    "candidate",
    "granted"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 3, type_codes, nullptr, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *AppendLogRequestTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_USHORT, 0, -1 },
    { flatbuffers::ET_SEQUENCE, 0, 0 },
    { flatbuffers::ET_SEQUENCE, 0, 0 },
    { flatbuffers::ET_SEQUENCE, 0, 0 },
    { flatbuffers::ET_UCHAR, 1, -1 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    IA20::Net::Engine::Raft::FB::LogEntryIdTypeTable
  };
  static const char * const names[] = {
    "leader",
    "matchLogEntry",
    "dataLogEntry",
    "leaderCommitEntry",
    "data"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 5, type_codes, type_refs, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *AppendLogResponseTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_SEQUENCE, 0, 0 },
    { flatbuffers::ET_SEQUENCE, 0, 0 },
    { flatbuffers::ET_BOOL, 0, -1 },
    { flatbuffers::ET_BOOL, 0, -1 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    IA20::Net::Engine::Raft::FB::LogEntryIdTypeTable
  };
  static const char * const names[] = {
    "matchLogEntry",
    "dataLogEntry",
    "success",
    "resend"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 4, type_codes, type_refs, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *MessageTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_SEQUENCE, 0, 0 },
    { flatbuffers::ET_UTYPE, 0, 1 },
    { flatbuffers::ET_SEQUENCE, 0, 1 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    IA20::Net::Engine::Raft::FB::HeaderTypeTable,
    IA20::Net::Engine::Raft::FB::ActionTypeTable
  };
  static const char * const names[] = {
    "header",
    "action_type",
    "action"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 3, type_codes, type_refs, nullptr, names
  };
  return &tt;
}

inline const IA20::Net::Engine::Raft::FB::Message *GetMessage(const void *buf) {
  return flatbuffers::GetRoot<IA20::Net::Engine::Raft::FB::Message>(buf);
}

inline const IA20::Net::Engine::Raft::FB::Message *GetSizePrefixedMessage(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<IA20::Net::Engine::Raft::FB::Message>(buf);
}

inline bool VerifyMessageBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<IA20::Net::Engine::Raft::FB::Message>(nullptr);
}

inline bool VerifySizePrefixedMessageBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<IA20::Net::Engine::Raft::FB::Message>(nullptr);
}

inline void FinishMessageBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<IA20::Net::Engine::Raft::FB::Message> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedMessageBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<IA20::Net::Engine::Raft::FB::Message> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace FB
}  // namespace Raft
}  // namespace Engine
}  // namespace Net
}  // namespace IA20

#endif  // FLATBUFFERS_GENERATED_RPC_IA20_NET_ENGINE_RAFT_FB_H_
