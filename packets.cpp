#include <iostream>
#include <pcap.h>
#include <cstring>
#include <sstream>
#include <string>
#include <fstream>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <cctype>

// Define Ethernet header structure.
struct ethheader {
    u_char ether_dhost[6]; // Destination MAC address
    u_char ether_shost[6]; // Source MAC address
    u_short ether_type;    // Protocol type (IP, ARP, etc.)
};

// Log a message to a file (append mode).
void log_to_file(const std::string &message) {
    std::ofstream logfile("traffic_log.txt", std::ios_base::app);
    if (logfile.is_open()) {
        logfile << message;
        logfile.close();
    } else {
        std::cerr << "Error opening traffic_log.txt for writing.\n";
    }
}

// A simple HTTP parser that logs the raw HTTP message.
// Note: For a production system, you would need proper TCP reassembly.
void parse_and_log_http(const std::string &httpPayload) {
    std::ostringstream entry;
    entry << "=== HTTP MESSAGE ===\n" << httpPayload << "\n=== END OF HTTP MESSAGE ===\n";
    log_to_file(entry.str());
    cout << "FOUND HTTP MESSAGE" << endl;
}

// This callback is called for every captured packet.
void packet_handler(unsigned char *args, const struct pcap_pkthdr *header, const unsigned char *packet) {
    std::ostringstream output;
    output << "\n---------------------------\nCaptured Packet:\n";

    // Parse and print Ethernet header.
    const ethheader *eth = reinterpret_cast<const ethheader*>(packet);
    output << "Ethernet Header:\n  Destination MAC: ";
    for (int i = 0; i < 6; ++i) {
        output << std::hex << static_cast<int>(eth->ether_dhost[i]);
        if (i < 5)
            output << ":";
    }
    output << "\n  Source MAC: ";
    for (int i = 0; i < 6; ++i) {
        output << std::hex << static_cast<int>(eth->ether_shost[i]);
        if (i < 5)
            output << ":";
    }
    output << "\n  Ether Type: " << std::dec << ntohs(eth->ether_type) << "\n";

    // Only process IP packets (Ethernet type 0x0800).
    if (ntohs(eth->ether_type) == 0x0800) {
        const ip *ip_hdr = reinterpret_cast<const ip*>(packet + sizeof(ethheader));
        output << "IP Header:\n  Source IP: " << inet_ntoa(ip_hdr->ip_src)
               << "\n  Destination IP: " << inet_ntoa(ip_hdr->ip_dst)
               << "\n  Protocol: " << static_cast<int>(ip_hdr->ip_p) << "\n";

        // Process TCP packets.
        if (ip_hdr->ip_p == IPPROTO_TCP) {
            unsigned int ip_header_length = ip_hdr->ip_hl * 4;
            const tcphdr *tcp_hdr = reinterpret_cast<const tcphdr*>(packet + sizeof(ethheader) + ip_header_length);
            output << "TCP Header:\n  Source Port: " << ntohs(tcp_hdr->source)
                   << "\n  Destination Port: " << ntohs(tcp_hdr->dest)
                   << "\n  Sequence Number: " << ntohl(tcp_hdr->seq)
                   << "\n  Ack Number: " << ntohl(tcp_hdr->ack_seq)
                   << "\n  TCP Header Length: " << (tcp_hdr->doff * 4) << " bytes\n";

            // Calculate where the payload begins.
            unsigned int tcp_header_length = tcp_hdr->doff * 4;
            const unsigned char *payload = packet + sizeof(ethheader) + ip_header_length + tcp_header_length;
            int payload_length = header->len - (sizeof(ethheader) + ip_header_length + tcp_header_length);

            if (payload_length > 0) {
                output << "Payload (" << payload_length << " bytes):\n";
                std::string payloadStr(reinterpret_cast<const char*>(payload), payload_length);

                // Check if the payload contains printable characters.
                bool isPrintable = true;
                for (int i = 0; i < payload_length; ++i) {
                    if (!std::isprint(payload[i]) && !std::isspace(payload[i])) {
                        isPrintable = false;
                        break;
                    }
                }
                if (isPrintable) {
                    output << payloadStr << "\n";
                } else {
                    // If not, output a hex dump.
                    for (int i = 0; i < payload_length; i++) {
                        output << std::hex << static_cast<int>(payload[i]) << " ";
                        if ((i + 1) % 16 == 0)
                            output << "\n";
                    }
                    output << std::dec << "\n";
                }

                // A very basic check for HTTP data.
                if (payload_length > 4 &&
                    (payloadStr.substr(0, 4) == "GET " ||
                     payloadStr.substr(0, 5) == "POST " ||
                     payloadStr.substr(0, 5) == "HTTP/")) {
                    parse_and_log_http(payloadStr);
                }
            }
        }
    }
    output << "---------------------------\n";
    log_to_file(output.str());
    std::cout << output.str();
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs;

    // Find all available devices.
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::cerr << "Error in pcap_findalldevs: " << errbuf << "\n";
        return 1;
    }
    if (alldevs == nullptr) {
        std::cerr << "No devices found! Exiting.\n";
        return 1;
    }

    // Use the first device in the list.
    pcap_if_t *device = alldevs;
    std::cout << "Using device: " << device->name << "\n";
    log_to_file("Using device: " + std::string(device->name) + "\n");

    // Open the selected device for live capture.
    pcap_t *handle = pcap_open_live(device->name, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        std::cerr << "Couldn't open device " << device->name << ": " << errbuf << "\n";
        pcap_freealldevs(alldevs);
        return 1;
    }

    // Set a filter to capture HTTP and HTTPS traffic (TCP port 80 or 443).
    struct bpf_program fp;
    char filter_exp[] = "tcp port 80 or tcp port 443";
    bpf_u_int32 net = 0; // Not used in this simple example.
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        std::cerr << "Couldn't parse filter " << filter_exp << ": " << pcap_geterr(handle) << "\n";
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 1;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        std::cerr << "Couldn't install filter " << filter_exp << ": " << pcap_geterr(handle) << "\n";
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 1;
    }

    std::cout << "Listening on " << device->name << " for web traffic...\n";
    log_to_file("Listening on " + std::string(device->name) + " for web traffic...\n");

    // Start capturing packets. This loop runs indefinitely until interrupted.
    pcap_loop(handle, 0, packet_handler, nullptr);

    // Cleanup.
    pcap_freealldevs(alldevs);
    pcap_close(handle);
    return 0;
}

