#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/parse_magnet_uri.hpp>
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: torrentd <magnet_link> <save_path>\n";
        return 1;
    }

    std::string magnet = argv[1];
    std::string save_path = argv[2];

    // Configure libtorrent settings pack for clearnet & local discovery
    libtorrent::settings_pack pack;
    pack.set_str(libtorrent::settings_pack::listen_interfaces, "0.0.0.0:6881");
    pack.set_bool(libtorrent::settings_pack::enable_dht, true);
    pack.set_bool(libtorrent::settings_pack::enable_lsd, true);
    pack.set_bool(libtorrent::settings_pack::enable_upnp, true);

    libtorrent::session ses(pack);

    libtorrent::error_code ec;
    libtorrent::add_torrent_params atp = libtorrent::parse_magnet_uri(magnet, ec);
    if (ec) {
        std::cerr << "Failed to parse magnet URI: " << ec.message() << "\n";
        return 1;
    }

    atp.save_path = save_path;
    libtorrent::torrent_handle h = ses.add_torrent(atp, ec);
    if (ec) {
        std::cerr << "Failed to add torrent: " << ec.message() << "\n";
        return 1;
    }

    std::cout << "torrentd: Active. Streaming to " << save_path << std::endl;

    // Main event loop to log progress and feed status to stdout
    while (true) {
        std::vector<libtorrent::alert*> alerts;
        ses.pop_alerts(&alerts);

        for (libtorrent::alert const* a : alerts) {
            if (auto st = libtorrent::alert_cast<libtorrent::state_update_alert>(a)) {
                for (auto const& th : st->status) {
                    std::cout << "PROGRESS:" << (th.progress * 100.0) 
                              << "|DL_RATE:" << (th.download_rate / 1024) << "KB/s" << std::endl;
                }
            }
        }

        ses.post_torrent_updates();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
