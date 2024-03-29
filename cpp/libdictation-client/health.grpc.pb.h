// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: health.proto
// Original file comments:
// Copyright 2015 The gRPC Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// The canonical version of this proto can be found at
// https://github.com/grpc/grpc-proto/blob/master/grpc/health/v1/health.proto
//
#ifndef GRPC_health_2eproto__INCLUDED
#define GRPC_health_2eproto__INCLUDED

#include "health.pb.h"

#include <functional>
#include <grpc/impl/codegen/port_platform.h>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace grpc {
namespace health {
namespace v1 {

class Health final {
 public:
  static constexpr char const* service_full_name() {
    return "grpc.health.v1.Health";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    // If the requested service is unknown, the call will fail with status
    // NOT_FOUND.
    virtual ::grpc::Status Check(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::health::v1::HealthCheckResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::grpc::health::v1::HealthCheckResponse>> AsyncCheck(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::grpc::health::v1::HealthCheckResponse>>(AsyncCheckRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::grpc::health::v1::HealthCheckResponse>> PrepareAsyncCheck(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::grpc::health::v1::HealthCheckResponse>>(PrepareAsyncCheckRaw(context, request, cq));
    }
    // Performs a watch for the serving status of the requested service.
    // The server will immediately send back a message indicating the current
    // serving status.  It will then subsequently send a new message whenever
    // the service's serving status changes.
    //
    // If the requested service is unknown when the call is received, the
    // server will send a message setting the serving status to
    // SERVICE_UNKNOWN but will *not* terminate the call.  If at some
    // future point, the serving status of the service becomes known, the
    // server will send a new message with the service's serving status.
    //
    // If the call terminates with status UNIMPLEMENTED, then clients
    // should assume this method is not supported and should not retry the
    // call.  If the call terminates with any other status (including OK),
    // clients should retry the call with appropriate exponential backoff.
    std::unique_ptr< ::grpc::ClientReaderInterface< ::grpc::health::v1::HealthCheckResponse>> Watch(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request) {
      return std::unique_ptr< ::grpc::ClientReaderInterface< ::grpc::health::v1::HealthCheckResponse>>(WatchRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::grpc::health::v1::HealthCheckResponse>> AsyncWatch(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::grpc::health::v1::HealthCheckResponse>>(AsyncWatchRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::grpc::health::v1::HealthCheckResponse>> PrepareAsyncWatch(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::grpc::health::v1::HealthCheckResponse>>(PrepareAsyncWatchRaw(context, request, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      // If the requested service is unknown, the call will fail with status
      // NOT_FOUND.
      virtual void Check(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest* request, ::grpc::health::v1::HealthCheckResponse* response, std::function<void(::grpc::Status)>) = 0;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void Check(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest* request, ::grpc::health::v1::HealthCheckResponse* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void Check(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest* request, ::grpc::health::v1::HealthCheckResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
      // Performs a watch for the serving status of the requested service.
      // The server will immediately send back a message indicating the current
      // serving status.  It will then subsequently send a new message whenever
      // the service's serving status changes.
      //
      // If the requested service is unknown when the call is received, the
      // server will send a message setting the serving status to
      // SERVICE_UNKNOWN but will *not* terminate the call.  If at some
      // future point, the serving status of the service becomes known, the
      // server will send a new message with the service's serving status.
      //
      // If the call terminates with status UNIMPLEMENTED, then clients
      // should assume this method is not supported and should not retry the
      // call.  If the call terminates with any other status (including OK),
      // clients should retry the call with appropriate exponential backoff.
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void Watch(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest* request, ::grpc::ClientReadReactor< ::grpc::health::v1::HealthCheckResponse>* reactor) = 0;
      #else
      virtual void Watch(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest* request, ::grpc::experimental::ClientReadReactor< ::grpc::health::v1::HealthCheckResponse>* reactor) = 0;
      #endif
    };
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    typedef class experimental_async_interface async_interface;
    #endif
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    async_interface* async() { return experimental_async(); }
    #endif
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::grpc::health::v1::HealthCheckResponse>* AsyncCheckRaw(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::grpc::health::v1::HealthCheckResponse>* PrepareAsyncCheckRaw(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientReaderInterface< ::grpc::health::v1::HealthCheckResponse>* WatchRaw(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::grpc::health::v1::HealthCheckResponse>* AsyncWatchRaw(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::grpc::health::v1::HealthCheckResponse>* PrepareAsyncWatchRaw(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status Check(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::health::v1::HealthCheckResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::grpc::health::v1::HealthCheckResponse>> AsyncCheck(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::grpc::health::v1::HealthCheckResponse>>(AsyncCheckRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::grpc::health::v1::HealthCheckResponse>> PrepareAsyncCheck(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::grpc::health::v1::HealthCheckResponse>>(PrepareAsyncCheckRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientReader< ::grpc::health::v1::HealthCheckResponse>> Watch(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request) {
      return std::unique_ptr< ::grpc::ClientReader< ::grpc::health::v1::HealthCheckResponse>>(WatchRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::grpc::health::v1::HealthCheckResponse>> AsyncWatch(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::grpc::health::v1::HealthCheckResponse>>(AsyncWatchRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::grpc::health::v1::HealthCheckResponse>> PrepareAsyncWatch(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::grpc::health::v1::HealthCheckResponse>>(PrepareAsyncWatchRaw(context, request, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      void Check(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest* request, ::grpc::health::v1::HealthCheckResponse* response, std::function<void(::grpc::Status)>) override;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void Check(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest* request, ::grpc::health::v1::HealthCheckResponse* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void Check(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest* request, ::grpc::health::v1::HealthCheckResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void Watch(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest* request, ::grpc::ClientReadReactor< ::grpc::health::v1::HealthCheckResponse>* reactor) override;
      #else
      void Watch(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest* request, ::grpc::experimental::ClientReadReactor< ::grpc::health::v1::HealthCheckResponse>* reactor) override;
      #endif
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::grpc::health::v1::HealthCheckResponse>* AsyncCheckRaw(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::grpc::health::v1::HealthCheckResponse>* PrepareAsyncCheckRaw(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientReader< ::grpc::health::v1::HealthCheckResponse>* WatchRaw(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request) override;
    ::grpc::ClientAsyncReader< ::grpc::health::v1::HealthCheckResponse>* AsyncWatchRaw(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncReader< ::grpc::health::v1::HealthCheckResponse>* PrepareAsyncWatchRaw(::grpc::ClientContext* context, const ::grpc::health::v1::HealthCheckRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_Check_;
    const ::grpc::internal::RpcMethod rpcmethod_Watch_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    // If the requested service is unknown, the call will fail with status
    // NOT_FOUND.
    virtual ::grpc::Status Check(::grpc::ServerContext* context, const ::grpc::health::v1::HealthCheckRequest* request, ::grpc::health::v1::HealthCheckResponse* response);
    // Performs a watch for the serving status of the requested service.
    // The server will immediately send back a message indicating the current
    // serving status.  It will then subsequently send a new message whenever
    // the service's serving status changes.
    //
    // If the requested service is unknown when the call is received, the
    // server will send a message setting the serving status to
    // SERVICE_UNKNOWN but will *not* terminate the call.  If at some
    // future point, the serving status of the service becomes known, the
    // server will send a new message with the service's serving status.
    //
    // If the call terminates with status UNIMPLEMENTED, then clients
    // should assume this method is not supported and should not retry the
    // call.  If the call terminates with any other status (including OK),
    // clients should retry the call with appropriate exponential backoff.
    virtual ::grpc::Status Watch(::grpc::ServerContext* context, const ::grpc::health::v1::HealthCheckRequest* request, ::grpc::ServerWriter< ::grpc::health::v1::HealthCheckResponse>* writer);
  };
  template <class BaseClass>
  class WithAsyncMethod_Check : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Check() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_Check() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Check(::grpc::ServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/, ::grpc::health::v1::HealthCheckResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestCheck(::grpc::ServerContext* context, ::grpc::health::v1::HealthCheckRequest* request, ::grpc::ServerAsyncResponseWriter< ::grpc::health::v1::HealthCheckResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_Watch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Watch() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_Watch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Watch(::grpc::ServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/, ::grpc::ServerWriter< ::grpc::health::v1::HealthCheckResponse>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestWatch(::grpc::ServerContext* context, ::grpc::health::v1::HealthCheckRequest* request, ::grpc::ServerAsyncWriter< ::grpc::health::v1::HealthCheckResponse>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncServerStreaming(1, context, request, writer, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_Check<WithAsyncMethod_Watch<Service > > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_Check : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_Check() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::health::v1::HealthCheckRequest, ::grpc::health::v1::HealthCheckResponse>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::health::v1::HealthCheckRequest* request, ::grpc::health::v1::HealthCheckResponse* response) { return this->Check(context, request, response); }));}
    void SetMessageAllocatorFor_Check(
        ::grpc::experimental::MessageAllocator< ::grpc::health::v1::HealthCheckRequest, ::grpc::health::v1::HealthCheckResponse>* allocator) {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
    #else
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::experimental().GetHandler(0);
    #endif
      static_cast<::grpc::internal::CallbackUnaryHandler< ::grpc::health::v1::HealthCheckRequest, ::grpc::health::v1::HealthCheckResponse>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_Check() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Check(::grpc::ServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/, ::grpc::health::v1::HealthCheckResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* Check(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/, ::grpc::health::v1::HealthCheckResponse* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* Check(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/, ::grpc::health::v1::HealthCheckResponse* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_Watch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_Watch() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(1,
          new ::grpc::internal::CallbackServerStreamingHandler< ::grpc::health::v1::HealthCheckRequest, ::grpc::health::v1::HealthCheckResponse>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::health::v1::HealthCheckRequest* request) { return this->Watch(context, request); }));
    }
    ~ExperimentalWithCallbackMethod_Watch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Watch(::grpc::ServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/, ::grpc::ServerWriter< ::grpc::health::v1::HealthCheckResponse>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerWriteReactor< ::grpc::health::v1::HealthCheckResponse>* Watch(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/)
    #else
    virtual ::grpc::experimental::ServerWriteReactor< ::grpc::health::v1::HealthCheckResponse>* Watch(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/)
    #endif
      { return nullptr; }
  };
  #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
  typedef ExperimentalWithCallbackMethod_Check<ExperimentalWithCallbackMethod_Watch<Service > > CallbackService;
  #endif

  typedef ExperimentalWithCallbackMethod_Check<ExperimentalWithCallbackMethod_Watch<Service > > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_Check : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Check() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_Check() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Check(::grpc::ServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/, ::grpc::health::v1::HealthCheckResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_Watch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Watch() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_Watch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Watch(::grpc::ServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/, ::grpc::ServerWriter< ::grpc::health::v1::HealthCheckResponse>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_Check : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Check() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_Check() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Check(::grpc::ServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/, ::grpc::health::v1::HealthCheckResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestCheck(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawMethod_Watch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Watch() {
      ::grpc::Service::MarkMethodRaw(1);
    }
    ~WithRawMethod_Watch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Watch(::grpc::ServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/, ::grpc::ServerWriter< ::grpc::health::v1::HealthCheckResponse>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestWatch(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncWriter< ::grpc::ByteBuffer>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncServerStreaming(1, context, request, writer, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_Check : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_Check() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->Check(context, request, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_Check() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Check(::grpc::ServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/, ::grpc::health::v1::HealthCheckResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* Check(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* Check(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_Watch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_Watch() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(1,
          new ::grpc::internal::CallbackServerStreamingHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const::grpc::ByteBuffer* request) { return this->Watch(context, request); }));
    }
    ~ExperimentalWithRawCallbackMethod_Watch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Watch(::grpc::ServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/, ::grpc::ServerWriter< ::grpc::health::v1::HealthCheckResponse>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerWriteReactor< ::grpc::ByteBuffer>* Watch(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/)
    #else
    virtual ::grpc::experimental::ServerWriteReactor< ::grpc::ByteBuffer>* Watch(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_Check : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_Check() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::grpc::health::v1::HealthCheckRequest, ::grpc::health::v1::HealthCheckResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::grpc::health::v1::HealthCheckRequest, ::grpc::health::v1::HealthCheckResponse>* streamer) {
                       return this->StreamedCheck(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_Check() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Check(::grpc::ServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/, ::grpc::health::v1::HealthCheckResponse* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedCheck(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::grpc::health::v1::HealthCheckRequest,::grpc::health::v1::HealthCheckResponse>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_Check<Service > StreamedUnaryService;
  template <class BaseClass>
  class WithSplitStreamingMethod_Watch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithSplitStreamingMethod_Watch() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::internal::SplitServerStreamingHandler<
          ::grpc::health::v1::HealthCheckRequest, ::grpc::health::v1::HealthCheckResponse>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerSplitStreamer<
                     ::grpc::health::v1::HealthCheckRequest, ::grpc::health::v1::HealthCheckResponse>* streamer) {
                       return this->StreamedWatch(context,
                         streamer);
                  }));
    }
    ~WithSplitStreamingMethod_Watch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status Watch(::grpc::ServerContext* /*context*/, const ::grpc::health::v1::HealthCheckRequest* /*request*/, ::grpc::ServerWriter< ::grpc::health::v1::HealthCheckResponse>* /*writer*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with split streamed
    virtual ::grpc::Status StreamedWatch(::grpc::ServerContext* context, ::grpc::ServerSplitStreamer< ::grpc::health::v1::HealthCheckRequest,::grpc::health::v1::HealthCheckResponse>* server_split_streamer) = 0;
  };
  typedef WithSplitStreamingMethod_Watch<Service > SplitStreamedService;
  typedef WithStreamedUnaryMethod_Check<WithSplitStreamingMethod_Watch<Service > > StreamedService;
};

}  // namespace v1
}  // namespace health
}  // namespace grpc


#endif  // GRPC_health_2eproto__INCLUDED
