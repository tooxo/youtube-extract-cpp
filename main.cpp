#include <iostream>
#include "youtube_video.h"
#include "youtube_playlist.h"

int main() {
    /**
     * This is a small test program to provide prove of work. Proper unit tests will follow.
     */
    std::string url_playlist{"https://www.youtube.com/playlist?list=PLSEW_LoxvVOaOltv3gwMblCkd1-ELiUd9"};
    youtube::Playlist playlist(url_playlist);

    for (std::pair<std::string, std::string> &song : playlist.videos) {
        std::cout << "ID: " << song.second << " TITLE: " << song.first << std::endl;
    }
    assert (playlist.videos.size() + playlist.skipped_songs == 277);
    std::cout << playlist.videos.size() << std::endl;

    std::string url_normal{"https://www.youtube.com/watch?v=KbdxRVuakqg"};
    std::string url_age_restricted{"https://www.youtube.com/watch?v=m6VXpTIrIAk"};
    std::string url_live{"https://www.youtube.com/watch?v=5qap5aO4i9A"};
    std::string invalid_video{"https://www.youtube.com/watch?v=apsdodsa"};
    std::string url_private{"https://www.youtube.com/watch?v=ozbnTScWups"};


    youtube::YouTube youTube(url_normal);

    youtube::StreamQuery query = youTube.streams();

    query.filter(youtube::FILTER_AUDIOCODEC, (std::string) "opus");

    for (youtube::Stream &stream : query) {
        assert(stream.audio_codec == "opus");
    }

    youtube::Stream *i_250 = query.get_by_itag(250);
    std::cout << i_250->itag << " " << i_250->url << std::endl;

    youtube::StreamQuery query1 = youTube.streams();
    std::cout << std::endl;
    query1.sort(youtube::FILTER_RESOLUTION);

    std::cout << std::endl;
    assert(youTube.title() == "Florence + The Machine - Sweet Nothing (Live From Austin City Limits)");
    assert(youTube.author() == "FlorenceMachineVEVO");
    assert(youTube.duration() == 259);

    youtube::YouTube youTube_age(url_age_restricted);

    assert(youTube_age.title() == "The Night Walker (self-injury drama AGE RESTRICTED)");
    std::cout << youTube_age.title() << std::endl;
    for (youtube::Stream &stream : youTube_age.fmt_streams) {
        assert(!stream.url.empty());
    }

    youtube::YouTube youtube_live(url_live);

    std::cout << youtube_live.title() << std::endl;
    for (youtube::Stream &stream : youtube_live.fmt_streams) {
        assert(!stream.url.empty());
    }

    bool excepted = false;
    try {
        youtube::YouTube youtube_unavailable(invalid_video);
    } catch (youtube::exceptions::VideoUnavailable &e) {
        std::cout << e.what() << std::endl;
        excepted = true;
    }
    assert(excepted);

    excepted = false;

    try {
        youtube::YouTube youTube_private(url_private);
    }
    catch (youtube::exceptions::VideoUnavailable &e) {
        std::cout << e.what() << std::endl;
        excepted = true;
    }
    assert(excepted);

    return 0;
}
