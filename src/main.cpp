#include <iostream>
#include <thread>
#include <chrono>
#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/alert_types.hpp>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <magnet_link> <save_path>\n";
        return 1;
    }

    std::string magnet = argv[1];
    std::string save_path = argv[2];

    lt::settings_pack pack;
    pack.set_str(lt::settings_pack::listen_interfaces, "0.0.0.0:6881");
    lt::session ses(pack);

    lt::error_code ec;
    lt::add_torrent_params atp = lt::parse_magnet_uri(magnet, ec);
    if (ec) {
        std::cerr << "Failed to parse magnet URI: " << ec.message() << "\n";
        return 1;
    }

    atp.save_path = save_path;
    lt::torrent_handle h = ses.add_torrent(atp);

    // CRITICAL FOR STREAMING: Enable sequential download
    h.set_sequential_download(true);
    std::cout << "[*] Torrent added. Sequential download enabled.\n";

    // Main monitoring and streaming loop
    bool running = true;
    while (running) {
        ses.pop_alerts_fn([](lt::alert* a) {
            if (auto st = lt::alert_cast<lt::state_update_alert>(a)) {
                for (auto const& p : st->status) {
                    std::cout << "[*] Progress: " << (p.progress * 100) 
                              << "% | Down Rate: " << (p.download_payload_rate / 1024) << " KB/s\n";
                }
            }
        });

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
