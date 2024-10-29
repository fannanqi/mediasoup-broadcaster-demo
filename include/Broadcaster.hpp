#ifndef BROADCASTER_H
#define BROADCASTER_H

#include "mediasoupclient.hpp"
#include "json.hpp"
#include <chrono>
#include <condition_variable>
#include <future>
#include <mutex>
#include <string>

class Broadcaster : public mediasoupclient::SendTransport::Listener,
                    mediasoupclient::RecvTransport::Listener,
                    mediasoupclient::Producer::Listener,
                    mediasoupclient::DataProducer::Listener,
                    mediasoupclient::DataConsumer::Listener,
                    // 消费者，提取对端音视频轨道
                    mediasoupclient::Consumer::Listener
{
public:
	struct TimerKiller
	{
		// returns false if killed:
		template<class R, class P>
		bool WaitFor(std::chrono::duration<R, P> const& time) const
		{
			std::unique_lock<std::mutex> lock(m);
			// 一旦线程进入等待状态，条件变量会自动释放持有的互斥锁，即传递给 cv.wait_for() 的
			// std::unique_lock<std::mutex>& lock。
			return !cv.wait_for(lock, time, [&] { return terminate; });
			// cv.wait_for(lock, time)，返回值是一个 std::cv_status 类型
			// cv.wait_for(lock, time, predicate)，指定的时间内反复检查该谓词是否为
			// true。如果谓词在等待期间被设置为 true，则立即返回 true；如果超时并且谓词仍为 false，则返回 false。
		}
		void Kill()
		{
			std::unique_lock<std::mutex> lock(m);
			terminate = true; // Should be modified inside mutex lock.
			cv.notify_all();  // It is safe, and *sometimes* optimal, to do this outside the lock.
		}

	private:
		mutable std::condition_variable cv;
		mutable std::mutex m;
		bool terminate = false;
	};

	/* Virtual methods inherited from SendTransport::Listener. */
public:
	// define TransportListener
	// 在连接建立时进行一些操作(该函数是一个异步操作)
	// 意味着这个函数将返回一个不包含任何值的异步操作结果（仅表示操作完成，而不返回数据）。
	std::future<void> OnConnect(
	  mediasoupclient::Transport* transport, const nlohmann::json& dtlsParameters) override;
	void OnConnectionStateChange(
	  mediasoupclient::Transport* transport, const std::string& connectionState) override;
	// end define TransportListener
	// define SendTransportListener
	std::future<std::string> OnProduce(
	  mediasoupclient::SendTransport* /*transport*/,
	  const std::string& kind,
	  nlohmann::json rtpParameters,
	  const nlohmann::json& appData) override;

	std::future<std::string> OnProduceData(
	  mediasoupclient::SendTransport* transport,
	  const nlohmann::json& sctpStreamParameters,
	  const std::string& label,
	  const std::string& protocol,
	  const nlohmann::json& appData) override;
	// end define SendTransportListener
	// 实现Consumer::Listener的虚函数：void OnTransportClose(Consumer* consumer) = 0;
	void OnTransportClose(mediasoupclient::Consumer* consumer) override;

	/* Virtual methods inherited from Producer::Listener. */
public:
	// define Producer::Listener 当此 producer 所属的 transport 因任何原因关闭时执行。生产者本身也是关闭的
	void OnTransportClose(mediasoupclient::Producer* producer) override;
	// end define Producer::Listener
	/* Virtual methods inherited from DataConsumer::Listener */
public:
	// 处理数据通道消息的回调
	// define DataConsumer::Listener
	void OnMessage(mediasoupclient::DataConsumer* dataConsumer, const webrtc::DataBuffer& buffer) override;
	void OnConnecting(mediasoupclient::DataConsumer* dataConsumer) override
	{
	}
	void OnClosing(mediasoupclient::DataConsumer* dataConsumer) override
	{
	}
	void OnClose(mediasoupclient::DataConsumer* dataConsumer) override
	{
	}
	void OnOpen(mediasoupclient::DataConsumer* dataConsumer) override
	{
	}
	void OnTransportClose(mediasoupclient::DataConsumer* dataConsumer) override
	{
	}
	// end define DataConsumer::Listener
	/* Virtual methods inherited from DataProducer::Listener */
public:
	// define DataProducer::Listener
	void OnOpen(mediasoupclient::DataProducer* dataProducer) override;
	void OnClose(mediasoupclient::DataProducer* dataProducer) override;
	void OnBufferedAmountChange(mediasoupclient::DataProducer* dataProducer, uint64_t size) override;
	void OnTransportClose(mediasoupclient::DataProducer* dataProducer) override;
	// end define DataProducer::Listener
public:
	void Start(
	  const std::string& baseUrl,
	  bool enableAudio,
	  bool useSimulcast,
	  const nlohmann::json& routerRtpCapabilities,
	  bool verifySsl = true);
	void Stop();

	~Broadcaster();

private:
	mediasoupclient::Device device;
	mediasoupclient::SendTransport* sendTransport{ nullptr };
	mediasoupclient::RecvTransport* recvTransport{ nullptr };
	mediasoupclient::DataProducer* dataProducer{ nullptr };
	mediasoupclient::DataConsumer* dataConsumer{ nullptr };
	mediasoupclient::Consumer* consumer{ nullptr };

	std::string id = std::to_string(rtc::CreateRandomId());
	std::string baseUrl;
	std::thread sendDataThread;

	struct TimerKiller timerKiller;
	bool verifySsl = true;

	std::future<void> OnConnectSendTransport(const nlohmann::json& dtlsParameters);
	std::future<void> OnConnectRecvTransport(const nlohmann::json& dtlsParameters);

	void CreateSendTransport(bool enableAudio, bool useSimulcast);
	void CreateRecvTransport();
	void CreateDataConsumer();
};

#endif // STOKER_HPP
