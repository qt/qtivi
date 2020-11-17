# Generated from media-simulation-server.pro.

#####################################################################
## ivimedia-simulation-server Tool:
#####################################################################

qt_get_tool_target_name(target_name ivimedia-simulation-server)
qt_add_tool(${target_name}
    SOURCES
        ../../plugins/ivimedia/media_simulator/database_helper.h
        ../../plugins/ivimedia/media_simulator/logging.cpp ../../plugins/ivimedia/media_simulator/logging.h
        ../../plugins/ivimedia/media_simulator/mediadiscoverybackend.cpp ../../plugins/ivimedia/media_simulator/mediadiscoverybackend.h
        ../../plugins/ivimedia/media_simulator/mediaindexerbackend.cpp ../../plugins/ivimedia/media_simulator/mediaindexerbackend.h
        ../../plugins/ivimedia/media_simulator/mediaplayerbackend.cpp ../../plugins/ivimedia/media_simulator/mediaplayerbackend.h
        ../../plugins/ivimedia/media_simulator/searchandbrowsebackend.cpp ../../plugins/ivimedia/media_simulator/searchandbrowsebackend.h
        ../../plugins/ivimedia/media_simulator/usbbrowsebackend.cpp ../../plugins/ivimedia/media_simulator/usbbrowsebackend.h
        ../../plugins/ivimedia/media_simulator/usbdevice.cpp ../../plugins/ivimedia/media_simulator/usbdevice.h
        core.cpp core.h
        main.cpp
        qivimediadiscoveryqtroadapter.cpp qivimediadiscoveryqtroadapter.h
        qivimediaindexerqtroadapter.cpp qivimediaindexerqtroadapter.h
        qivimediaplayerqtroadapter.cpp qivimediaplayerqtroadapter.h
        qivisearchandbrowsemodelqtroadapter.cpp qivisearchandbrowsemodelqtroadapter.h
    INCLUDE_DIRECTORIES
        ../../plugins/ivimedia/media_simulator
    PUBLIC_LIBRARIES
        Qt::IviCore
        Qt::IviMedia
        Qt::Multimedia
        Qt::RemoteObjects
        Qt::Sql
        iviremoteobjects_helper_private
)
qt6_add_repc_source(ivimedia-simulation-server
    qivimediaindexer.rep
    qivimediaplayer.rep
    qivimediadiscoverymodel.rep
    qivisearchandbrowsemodel.rep
)

#### Keys ignored in scope 1:.:.:media-simulation-server.pro:<TRUE>:
# TEMPLATE = "app"

## Scopes:
#####################################################################

qt_extend_target(${target_name} CONDITION QT_FEATURE_system_taglib
    PUBLIC_LIBRARIES
        taglib
)

qt_extend_target(${target_name} CONDITION QT_FEATURE_taglib AND NOT QT_FEATURE_system_taglib
    SOURCES
        ../../3rdparty/taglib/taglib/taglib/ape/apefile.cpp ../../3rdparty/taglib/taglib/taglib/ape/apefile.h
        ../../3rdparty/taglib/taglib/taglib/ape/apefooter.cpp ../../3rdparty/taglib/taglib/taglib/ape/apefooter.h
        ../../3rdparty/taglib/taglib/taglib/ape/apeitem.cpp ../../3rdparty/taglib/taglib/taglib/ape/apeitem.h
        ../../3rdparty/taglib/taglib/taglib/ape/apeproperties.cpp ../../3rdparty/taglib/taglib/taglib/ape/apeproperties.h
        ../../3rdparty/taglib/taglib/taglib/ape/apetag.cpp ../../3rdparty/taglib/taglib/taglib/ape/apetag.h
        ../../3rdparty/taglib/taglib/taglib/asf/asfattribute.cpp ../../3rdparty/taglib/taglib/taglib/asf/asfattribute.h
        ../../3rdparty/taglib/taglib/taglib/asf/asffile.cpp ../../3rdparty/taglib/taglib/taglib/asf/asffile.h
        ../../3rdparty/taglib/taglib/taglib/asf/asfpicture.cpp ../../3rdparty/taglib/taglib/taglib/asf/asfpicture.h
        ../../3rdparty/taglib/taglib/taglib/asf/asfproperties.cpp ../../3rdparty/taglib/taglib/taglib/asf/asfproperties.h
        ../../3rdparty/taglib/taglib/taglib/asf/asftag.cpp ../../3rdparty/taglib/taglib/taglib/asf/asftag.h
        ../../3rdparty/taglib/taglib/taglib/asf/asfutils.h
        ../../3rdparty/taglib/taglib/taglib/audioproperties.cpp ../../3rdparty/taglib/taglib/taglib/audioproperties.h
        ../../3rdparty/taglib/taglib/taglib/fileref.cpp ../../3rdparty/taglib/taglib/taglib/fileref.h
        ../../3rdparty/taglib/taglib/taglib/flac/flacfile.cpp ../../3rdparty/taglib/taglib/taglib/flac/flacfile.h
        ../../3rdparty/taglib/taglib/taglib/flac/flacmetadatablock.cpp ../../3rdparty/taglib/taglib/taglib/flac/flacmetadatablock.h
        ../../3rdparty/taglib/taglib/taglib/flac/flacpicture.cpp ../../3rdparty/taglib/taglib/taglib/flac/flacpicture.h
        ../../3rdparty/taglib/taglib/taglib/flac/flacproperties.cpp ../../3rdparty/taglib/taglib/taglib/flac/flacproperties.h
        ../../3rdparty/taglib/taglib/taglib/flac/flacunknownmetadatablock.cpp ../../3rdparty/taglib/taglib/taglib/flac/flacunknownmetadatablock.h
        ../../3rdparty/taglib/taglib/taglib/it/itfile.cpp ../../3rdparty/taglib/taglib/taglib/it/itfile.h
        ../../3rdparty/taglib/taglib/taglib/it/itproperties.cpp ../../3rdparty/taglib/taglib/taglib/it/itproperties.h
        ../../3rdparty/taglib/taglib/taglib/mod/modfile.cpp ../../3rdparty/taglib/taglib/taglib/mod/modfile.h
        ../../3rdparty/taglib/taglib/taglib/mod/modfilebase.cpp ../../3rdparty/taglib/taglib/taglib/mod/modfilebase.h
        ../../3rdparty/taglib/taglib/taglib/mod/modfileprivate.h
        ../../3rdparty/taglib/taglib/taglib/mod/modproperties.cpp ../../3rdparty/taglib/taglib/taglib/mod/modproperties.h
        ../../3rdparty/taglib/taglib/taglib/mod/modtag.cpp ../../3rdparty/taglib/taglib/taglib/mod/modtag.h
        ../../3rdparty/taglib/taglib/taglib/mp4/mp4atom.cpp ../../3rdparty/taglib/taglib/taglib/mp4/mp4atom.h
        ../../3rdparty/taglib/taglib/taglib/mp4/mp4coverart.cpp ../../3rdparty/taglib/taglib/taglib/mp4/mp4coverart.h
        ../../3rdparty/taglib/taglib/taglib/mp4/mp4file.cpp ../../3rdparty/taglib/taglib/taglib/mp4/mp4file.h
        ../../3rdparty/taglib/taglib/taglib/mp4/mp4item.cpp ../../3rdparty/taglib/taglib/taglib/mp4/mp4item.h
        ../../3rdparty/taglib/taglib/taglib/mp4/mp4properties.cpp ../../3rdparty/taglib/taglib/taglib/mp4/mp4properties.h
        ../../3rdparty/taglib/taglib/taglib/mp4/mp4tag.cpp ../../3rdparty/taglib/taglib/taglib/mp4/mp4tag.h
        ../../3rdparty/taglib/taglib/taglib/mpc/mpcfile.cpp ../../3rdparty/taglib/taglib/taglib/mpc/mpcfile.h
        ../../3rdparty/taglib/taglib/taglib/mpc/mpcproperties.cpp ../../3rdparty/taglib/taglib/taglib/mpc/mpcproperties.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v1/id3v1genres.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v1/id3v1genres.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v1/id3v1tag.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v1/id3v1tag.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/attachedpictureframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/attachedpictureframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/chapterframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/chapterframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/commentsframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/commentsframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/eventtimingcodesframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/eventtimingcodesframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/generalencapsulatedobjectframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/generalencapsulatedobjectframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/ownershipframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/ownershipframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/podcastframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/podcastframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/popularimeterframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/popularimeterframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/privateframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/privateframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/relativevolumeframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/relativevolumeframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/synchronizedlyricsframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/synchronizedlyricsframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/tableofcontentsframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/tableofcontentsframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/textidentificationframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/textidentificationframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/uniquefileidentifierframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/uniquefileidentifierframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/unknownframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/unknownframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/unsynchronizedlyricsframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/unsynchronizedlyricsframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/urllinkframe.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames/urllinkframe.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/id3v2extendedheader.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/id3v2extendedheader.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/id3v2footer.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/id3v2footer.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/id3v2frame.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/id3v2frame.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/id3v2framefactory.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/id3v2framefactory.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/id3v2header.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/id3v2header.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/id3v2synchdata.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/id3v2synchdata.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/id3v2tag.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/id3v2tag.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/mpegfile.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/mpegfile.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/mpegheader.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/mpegheader.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/mpegproperties.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/mpegproperties.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/mpegutils.h
        ../../3rdparty/taglib/taglib/taglib/mpeg/xingheader.cpp ../../3rdparty/taglib/taglib/taglib/mpeg/xingheader.h
        ../../3rdparty/taglib/taglib/taglib/ogg/flac/oggflacfile.cpp ../../3rdparty/taglib/taglib/taglib/ogg/flac/oggflacfile.h
        ../../3rdparty/taglib/taglib/taglib/ogg/oggfile.cpp ../../3rdparty/taglib/taglib/taglib/ogg/oggfile.h
        ../../3rdparty/taglib/taglib/taglib/ogg/oggpage.cpp ../../3rdparty/taglib/taglib/taglib/ogg/oggpage.h
        ../../3rdparty/taglib/taglib/taglib/ogg/oggpageheader.cpp ../../3rdparty/taglib/taglib/taglib/ogg/oggpageheader.h
        ../../3rdparty/taglib/taglib/taglib/ogg/opus/opusfile.cpp ../../3rdparty/taglib/taglib/taglib/ogg/opus/opusfile.h
        ../../3rdparty/taglib/taglib/taglib/ogg/opus/opusproperties.cpp ../../3rdparty/taglib/taglib/taglib/ogg/opus/opusproperties.h
        ../../3rdparty/taglib/taglib/taglib/ogg/speex/speexfile.cpp ../../3rdparty/taglib/taglib/taglib/ogg/speex/speexfile.h
        ../../3rdparty/taglib/taglib/taglib/ogg/speex/speexproperties.cpp ../../3rdparty/taglib/taglib/taglib/ogg/speex/speexproperties.h
        ../../3rdparty/taglib/taglib/taglib/ogg/vorbis/vorbisfile.cpp ../../3rdparty/taglib/taglib/taglib/ogg/vorbis/vorbisfile.h
        ../../3rdparty/taglib/taglib/taglib/ogg/vorbis/vorbisproperties.cpp ../../3rdparty/taglib/taglib/taglib/ogg/vorbis/vorbisproperties.h
        ../../3rdparty/taglib/taglib/taglib/ogg/xiphcomment.cpp ../../3rdparty/taglib/taglib/taglib/ogg/xiphcomment.h
        ../../3rdparty/taglib/taglib/taglib/riff/aiff/aifffile.cpp ../../3rdparty/taglib/taglib/taglib/riff/aiff/aifffile.h
        ../../3rdparty/taglib/taglib/taglib/riff/aiff/aiffproperties.cpp ../../3rdparty/taglib/taglib/taglib/riff/aiff/aiffproperties.h
        ../../3rdparty/taglib/taglib/taglib/riff/rifffile.cpp ../../3rdparty/taglib/taglib/taglib/riff/rifffile.h
        ../../3rdparty/taglib/taglib/taglib/riff/riffutils.h
        ../../3rdparty/taglib/taglib/taglib/riff/wav/infotag.cpp ../../3rdparty/taglib/taglib/taglib/riff/wav/infotag.h
        ../../3rdparty/taglib/taglib/taglib/riff/wav/wavfile.cpp ../../3rdparty/taglib/taglib/taglib/riff/wav/wavfile.h
        ../../3rdparty/taglib/taglib/taglib/riff/wav/wavproperties.cpp ../../3rdparty/taglib/taglib/taglib/riff/wav/wavproperties.h
        ../../3rdparty/taglib/taglib/taglib/s3m/s3mfile.cpp ../../3rdparty/taglib/taglib/taglib/s3m/s3mfile.h
        ../../3rdparty/taglib/taglib/taglib/s3m/s3mproperties.cpp ../../3rdparty/taglib/taglib/taglib/s3m/s3mproperties.h
        ../../3rdparty/taglib/taglib/taglib/tag.cpp ../../3rdparty/taglib/taglib/taglib/tag.h
        ../../3rdparty/taglib/taglib/taglib/taglib_export.h
        ../../3rdparty/taglib/taglib/taglib/tagunion.cpp ../../3rdparty/taglib/taglib/taglib/tagunion.h
        ../../3rdparty/taglib/taglib/taglib/tagutils.cpp ../../3rdparty/taglib/taglib/taglib/tagutils.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/taglib.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/tbytevector.cpp ../../3rdparty/taglib/taglib/taglib/toolkit/tbytevector.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/tbytevectorlist.cpp ../../3rdparty/taglib/taglib/taglib/toolkit/tbytevectorlist.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/tbytevectorstream.cpp ../../3rdparty/taglib/taglib/taglib/toolkit/tbytevectorstream.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/tdebug.cpp ../../3rdparty/taglib/taglib/taglib/toolkit/tdebug.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/tdebuglistener.cpp ../../3rdparty/taglib/taglib/taglib/toolkit/tdebuglistener.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/tfile.cpp ../../3rdparty/taglib/taglib/taglib/toolkit/tfile.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/tfilestream.cpp ../../3rdparty/taglib/taglib/taglib/toolkit/tfilestream.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/tiostream.cpp ../../3rdparty/taglib/taglib/taglib/toolkit/tiostream.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/tlist.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/tmap.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/tpropertymap.cpp ../../3rdparty/taglib/taglib/taglib/toolkit/tpropertymap.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/trefcounter.cpp ../../3rdparty/taglib/taglib/taglib/toolkit/trefcounter.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/tstring.cpp ../../3rdparty/taglib/taglib/taglib/toolkit/tstring.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/tstringlist.cpp ../../3rdparty/taglib/taglib/taglib/toolkit/tstringlist.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/tutils.h
        ../../3rdparty/taglib/taglib/taglib/toolkit/tzlib.cpp ../../3rdparty/taglib/taglib/taglib/toolkit/tzlib.h
        ../../3rdparty/taglib/taglib/taglib/trueaudio/trueaudiofile.cpp ../../3rdparty/taglib/taglib/taglib/trueaudio/trueaudiofile.h
        ../../3rdparty/taglib/taglib/taglib/trueaudio/trueaudioproperties.cpp ../../3rdparty/taglib/taglib/taglib/trueaudio/trueaudioproperties.h
        ../../3rdparty/taglib/taglib/taglib/wavpack/wavpackfile.cpp ../../3rdparty/taglib/taglib/taglib/wavpack/wavpackfile.h
        ../../3rdparty/taglib/taglib/taglib/wavpack/wavpackproperties.cpp ../../3rdparty/taglib/taglib/taglib/wavpack/wavpackproperties.h
        ../../3rdparty/taglib/taglib/taglib/xm/xmfile.cpp ../../3rdparty/taglib/taglib/taglib/xm/xmfile.h
        ../../3rdparty/taglib/taglib/taglib/xm/xmproperties.cpp ../../3rdparty/taglib/taglib/taglib/xm/xmproperties.h
    DEFINES
        TAGLIB_STATIC
    INCLUDE_DIRECTORIES
        ../../3rdparty/taglib
        ../../3rdparty/taglib/taglib
        ../../3rdparty/taglib/taglib/taglib
        ../../3rdparty/taglib/taglib/taglib/ape
        ../../3rdparty/taglib/taglib/taglib/asf
        ../../3rdparty/taglib/taglib/taglib/flac
        ../../3rdparty/taglib/taglib/taglib/it
        ../../3rdparty/taglib/taglib/taglib/mod
        ../../3rdparty/taglib/taglib/taglib/mp4
        ../../3rdparty/taglib/taglib/taglib/mpc
        ../../3rdparty/taglib/taglib/taglib/mpeg
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v1
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2
        ../../3rdparty/taglib/taglib/taglib/mpeg/id3v2/frames
        ../../3rdparty/taglib/taglib/taglib/ogg
        ../../3rdparty/taglib/taglib/taglib/ogg/flac
        ../../3rdparty/taglib/taglib/taglib/ogg/opus
        ../../3rdparty/taglib/taglib/taglib/ogg/speex
        ../../3rdparty/taglib/taglib/taglib/ogg/vorbis
        ../../3rdparty/taglib/taglib/taglib/riff
        ../../3rdparty/taglib/taglib/taglib/riff/aiff
        ../../3rdparty/taglib/taglib/taglib/riff/wav
        ../../3rdparty/taglib/taglib/taglib/s3m
        ../../3rdparty/taglib/taglib/taglib/toolkit
        ../../3rdparty/taglib/taglib/taglib/trueaudio
        ../../3rdparty/taglib/taglib/taglib/wavpack
        ../../3rdparty/taglib/taglib/taglib/xm
)

#### Keys ignored in scope 7:.:../../plugins/ivimedia/media_simulator/../../../3rdparty/taglib:../../plugins/ivimedia/media_simulator/../../../3rdparty/taglib/taglib.pri:win32-msvc_x_:
# QMAKE_CXXFLAGS_WARN_ON = "/wd4100"

#### Keys ignored in scope 8:.:../../plugins/ivimedia/media_simulator/../../../3rdparty/taglib:../../plugins/ivimedia/media_simulator/../../../3rdparty/taglib/taglib.pri:GCC:
# QMAKE_CFLAGS_WARN_ON = "-Wno-unused-parameter" "-Wno-main"

#### Keys ignored in scope 9:.:../../plugins/ivimedia/media_simulator/../../../3rdparty/taglib:../../plugins/ivimedia/media_simulator/../../../3rdparty/taglib/taglib.pri:CLANG:
# QMAKE_CXXFLAGS_WARN_ON = "-Wno-unused-parameter" "-Wno-deprecated-declarations" "-Wno-sign-compare" "-Wno-delete-non-virtual-dtor"

qt_extend_target(${target_name} CONDITION QT_FEATURE_taglib AND UNIX AND NOT QT_FEATURE_system_taglib
    SOURCES
        ../../3rdparty/taglib/taglib/taglib/toolkit/unicode.cpp
)
