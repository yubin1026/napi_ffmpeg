#include <node_api.h>
#include <stdio.h>
#include <stdint.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavutil/avstring.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>

#define NAPI_DEFINE_CONSTANT(target, constant)                           \
  do {                                                                   \
    napi_value v = NULL;                                                 \
    napi_create_int32(env, constant, &v);                                \
    napi_set_named_property(env, target, #constant, v);                  \
  }                                                                      \
  while (0)


#define NAPI_SET_METHOD(target, name, callback)                         \
  do {                                                                  \
    napi_value fn = NULL;                                               \
    napi_create_function(env, NULL, 0, callback, NULL, &fn);            \
    napi_set_named_property(env, target, name, fn);                     \
  }                                                                     \
  while (0)


napi_value av_register_all_Callback(napi_env env, napi_callback_info info) {

  av_register_all();
  
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value av_dump_disable_Callback(napi_env env, napi_callback_info info) {

  freopen("/dev/null", "w", stderr);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value avformat_open_input_Callback(napi_env env, napi_callback_info info) {

  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  char input_filename[256];
  size_t len;
  napi_get_value_string_utf8(env, argv[0], input_filename, 256, &len);

  AVFormatContext *ic = NULL;
  AVInputFormat *iformat = NULL;

  avformat_open_input(&ic, input_filename, iformat, NULL);

  napi_value ret;
  napi_create_int64(env, (int64_t) ic, &ret);
  return ret;
}

napi_value av_dump_format_Callback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  char input_filename[256];
  size_t len;
  napi_get_value_string_utf8(env, argv[1], input_filename, 256, &len);

  av_dump_format((AVFormatContext*) arg0, 0, input_filename, 0);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value avformat_find_stream_info_Callback(napi_env env, napi_callback_info info) {

  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);
  AVFormatContext* ic  = (AVFormatContext*) arg0;

  avformat_find_stream_info(ic, NULL);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}


napi_value av_find_best_stream_Callback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  int arg1;
  napi_get_value_int32(env, argv[1], &arg1);

  //int stream = av_find_best_stream((AVFormatContext*) arg0, (AVMediaType)arg1, -1, -1, NULL, 0);
  int stream = av_find_best_stream((AVFormatContext*) arg0, arg1, -1, -1, NULL, 0);

  napi_value ret;
  napi_create_int32(env, (int32_t) stream, &ret);
  return ret;
}

napi_value avformat_stream_Callback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  int arg1;
  napi_get_value_int32(env, argv[1], &arg1);

  AVFormatContext* ic = (AVFormatContext*) arg0;

  AVStream* stream = ic->streams[arg1];

  napi_value ret;
  napi_create_int64(env, (int64_t) stream, &ret);

  return ret;
}

napi_value avformat_context_Callback(napi_env env, napi_callback_info info) {

  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  int arg1;
  napi_get_value_int32(env, argv[1], &arg1);

  AVFormatContext* ic = (AVFormatContext*) arg0;

  AVCodecContext* codec_ctx = ic->streams[arg1]->codec;
  codec_ctx->get_buffer2 = avcodec_default_get_buffer2;

  napi_value ret;
  napi_create_int64(env, (int64_t) codec_ctx, &ret);
  return ret;
}

////

napi_value avcodec_dimension_Callback(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);
    
  AVCodecContext* codec_ctx  = (AVCodecContext*) arg0;

  napi_value ret;
  napi_create_array_with_length(env, 2, &ret);

  napi_value e;
  napi_create_int32(env, codec_ctx->width, &e);
  napi_set_element(env, ret, 0, e);

  napi_create_int32(env, codec_ctx->height, &e);
  napi_set_element(env, ret, 1, e);

  return ret;
}

napi_value avcodec_open_Callback(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);
    
  AVCodecContext* codec_ctx  = (AVCodecContext*) arg0;

  AVCodec* codec = avcodec_find_decoder(codec_ctx->codec_id);
  avcodec_open2(codec_ctx, codec, NULL);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value avcodec_close_Callback(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);
    
  AVCodecContext* codec_ctx  = (AVCodecContext*) arg0;

  avcodec_close(codec_ctx);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value avformat_close_input_Callback(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);
    
  AVFormatContext* ic  = (AVFormatContext*) arg0;

  avformat_close_input(&ic);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

static struct SwrContext* swr;
static uint8_t** destBuffer = NULL;
static int destBufferLinesize;
static uint8_t audio_buffer[192000];


napi_value avcodec_decode_Callback(napi_env env, napi_callback_info info) {
  size_t argc = 5;
  napi_value argv[5];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);
  AVFormatContext* ic  = (AVFormatContext*) arg0;

  int64_t arg1;
  napi_get_value_int64(env, argv[1], &arg1);
  AVCodecContext* vcodec_ctx  = (AVCodecContext*) arg1;

  int vstream;
  napi_get_value_int32(env, argv[2], &vstream);

  int64_t arg3;
  napi_get_value_int64(env, argv[3], &arg3);
  AVCodecContext* acodec_ctx  = (AVCodecContext*) arg3;

  int astream;
  napi_get_value_int32(env, argv[4], &astream);

  int64_t arg5;
  int sstream;
  

  AVFrame* frame = av_frame_alloc();
  AVPacket packet;
  uint8_t* bmp = NULL;
  int bmp_len = 0;
  int audio_len = 0;
  int ret = -1;
  int repeat_pict = 0;
  double pts;
  while(1) {
      ret = av_read_frame(ic, &packet);
      if(ret < 0) break;
      if(packet.stream_index == vstream) {
        
          int got_frame = 0;
          pts = 0;
          avcodec_decode_video2(vcodec_ctx, frame, &got_frame, &packet);
          if(got_frame) {
              struct SwsContext* sws_ctx = NULL;
              sws_ctx = sws_getContext(vcodec_ctx->width, vcodec_ctx->height, vcodec_ctx->pix_fmt, 
                vcodec_ctx->width, vcodec_ctx->height,
                AV_PIX_FMT_RGB32_1, SWS_BICUBIC, NULL, NULL, NULL);
              
            if(packet.dts != AV_NOPTS_VALUE) {
              pts = av_frame_get_best_effort_timestamp(frame);
            } else {
              pts = 0;
            }
            repeat_pict = frame->repeat_pict;
            pts *= av_q2d(ic->streams[vstream]->time_base);
            
            int numBytes = avpicture_get_size(AV_PIX_FMT_RGB32_1, vcodec_ctx->width, vcodec_ctx->height);
            uint8_t* pointers[4];
            int linesizes[4];

            av_image_alloc(pointers, linesizes, vcodec_ctx->width, vcodec_ctx->height, AV_PIX_FMT_RGB32_1, 1);
            sws_scale(sws_ctx, (uint8_t const * const *)frame->data, frame->linesize, 0, vcodec_ctx->height, pointers, linesizes);
            bmp = pointers[0];
            bmp_len = linesizes[0] * vcodec_ctx->height;

            break;
          }
          
      } else if(packet.stream_index == astream) {
          bmp = NULL;
          int got_frame = 0;
          // decode frame
          int decoded = avcodec_decode_audio4(acodec_ctx, frame, &got_frame, &packet);
          if (decoded < 0) {
              break;
          }
          if(got_frame) {
              if(packet.pts != AV_NOPTS_VALUE) {
                pts = packet.pts;
              }
              pts *= av_q2d(ic->streams[astream]->time_base);

              // resample frame
              audio_len = av_samples_get_buffer_size(NULL, 2, frame->nb_samples, AV_SAMPLE_FMT_S16, 1);
              int frame_count = swr_convert(swr, destBuffer, destBufferLinesize,
                                            (const uint8_t**) frame->extended_data, frame->nb_samples);

              memcpy(audio_buffer, destBuffer[0], audio_len);
              break;
          }
      } else {
        // subtitle stream...
      }
      av_packet_unref(&packet);
  }
  av_free_packet(&packet);
  av_frame_free(&frame);
  //av_frame_free(&frameRGB);
  
  napi_value obj;

  if(ret == 0) {
    napi_create_object(env, &obj);

    if(bmp != NULL) {
      napi_value v;
      napi_create_int32(env, 0, &v);
      napi_set_named_property(env, obj, "type", v);     

      void* buffer;
      napi_create_buffer_copy(env, bmp_len, bmp, &buffer, &v);
      napi_set_named_property(env, obj, "buffer", v);     
      av_free(bmp);

      napi_create_double(env, pts, &v);
      napi_set_named_property(env, obj, "pts", v);     

      napi_create_int32(env, repeat_pict, &v);
      napi_set_named_property(env, obj, "repeat_pict", v);     

    } else {

      napi_value v;
      napi_create_int32(env, 1, &v);
      napi_set_named_property(env, obj, "type", v);     

      void* buffer;
      napi_create_buffer_copy(env, audio_len, audio_buffer, &buffer, &v);
      napi_set_named_property(env, obj, "buffer", v);     

      napi_create_double(env, pts, &v);
      napi_set_named_property(env, obj, "pts", v);     

    }
  } else {
    napi_get_undefined(env, &obj);
  }

  return obj;
}

napi_value avbuffer_free_Callback(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);

  uint8_t* data;
  size_t len;
  napi_get_buffer_info(env, argv[0], (void**) &data, &len);
  fprintf(stdout, "buffer len is %d\n", len);

  //av_free(data);
  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value av_usleep_Callback(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  double arg0;
  napi_get_value_double(env, argv[0], &arg0);

  av_usleep(arg0);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

napi_value av_gettime_Callback(napi_env env, napi_callback_info info) {
 
  double v = (double) av_gettime() / 1000000.0; // return in second

  napi_value ret;
  napi_create_double(env, v, &ret);
  return ret;
}


napi_value avstream_timebase_Callback(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);
    
  AVStream* stream = (AVStream*) arg0;

  napi_value ret;
  napi_create_double(env, av_q2d(stream->time_base), &ret);
  return ret;
}


napi_value avcodec_timebase_Callback(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);
    
  AVCodecContext* codec_ctx  = (AVCodecContext*) arg0;

  napi_value ret;
  napi_create_double(env, av_q2d(codec_ctx->time_base), &ret);
  return ret;
}

napi_value avcodec_sample_rate_Callback(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);
    
  AVCodecContext* codec_ctx  = (AVCodecContext*) arg0;

  napi_value ret;
  napi_create_int32(env, codec_ctx->sample_rate, &ret);
  return ret;
}

napi_value avcodec_resampler_Callback(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  int64_t arg0;
  napi_get_value_int64(env, argv[0], &arg0);
  AVCodecContext* codec_ctx  = (AVCodecContext*) arg0;

  //struct SwrContext* swr = swr_alloc();
  swr = swr_alloc();

  av_opt_set_int(swr, "in_channel_count",  codec_ctx->channels, 0);
  av_opt_set_int(swr, "out_channel_count", 2, 0);

  av_opt_set_int(swr, "in_channel_layout",  codec_ctx->channel_layout, 0);
  av_opt_set_int(swr, "out_channel_layout", AV_CH_LAYOUT_STEREO, 0);

  av_opt_set_int(swr, "in_sample_rate", codec_ctx->sample_rate, 0);
  av_opt_set_int(swr, "out_sample_rate", codec_ctx->sample_rate, 0);

  av_opt_set_sample_fmt(swr, "in_sample_fmt",  codec_ctx->sample_fmt, 0);
  av_opt_set_sample_fmt(swr, "out_sample_fmt", AV_SAMPLE_FMT_S16,  0);  // planar format


  swr_init(swr);
  av_samples_alloc_array_and_samples( &destBuffer,
                                   &destBufferLinesize,
                                   2,
                                   2048,
                                   AV_SAMPLE_FMT_S16,
                                   1);

  napi_value ret;
  napi_get_undefined(env, &ret);
  return ret;
}

/////
napi_value Init(napi_env env, napi_value exports) {

  NAPI_DEFINE_CONSTANT(exports, AVMEDIA_TYPE_VIDEO);
  NAPI_DEFINE_CONSTANT(exports, AVMEDIA_TYPE_AUDIO);
  NAPI_DEFINE_CONSTANT(exports, AVMEDIA_TYPE_SUBTITLE);
  
  NAPI_SET_METHOD(exports, "av_register_all", av_register_all_Callback);
  NAPI_SET_METHOD(exports, "avformat_open_input", avformat_open_input_Callback);
  NAPI_SET_METHOD(exports, "av_dump_format", av_dump_format_Callback);
  NAPI_SET_METHOD(exports, "avformat_find_stream_info", avformat_find_stream_info_Callback);
  NAPI_SET_METHOD(exports, "av_find_best_stream", av_find_best_stream_Callback);
  NAPI_SET_METHOD(exports, "avformat_stream", avformat_stream_Callback);
  NAPI_SET_METHOD(exports, "avformat_context", avformat_context_Callback);
  NAPI_SET_METHOD(exports, "avcodec_close", avcodec_close_Callback);
  NAPI_SET_METHOD(exports, "avformat_close_input", avformat_close_input_Callback);

  NAPI_SET_METHOD(exports, "av_usleep", av_usleep_Callback);
  NAPI_SET_METHOD(exports, "av_gettime", av_gettime_Callback);

  NAPI_SET_METHOD(exports, "avcodec_open", avcodec_open_Callback);
  NAPI_SET_METHOD(exports, "avcodec_decode", avcodec_decode_Callback);
  NAPI_SET_METHOD(exports, "avcodec_dimension", avcodec_dimension_Callback);
  NAPI_SET_METHOD(exports, "avcodec_resampler", avcodec_resampler_Callback);
  NAPI_SET_METHOD(exports, "avcodec_sample_rate", avcodec_sample_rate_Callback);
  NAPI_SET_METHOD(exports, "avstream_timebase", avstream_timebase_Callback);
  NAPI_SET_METHOD(exports, "avcodec_timebase", avcodec_timebase_Callback);
  NAPI_SET_METHOD(exports, "avbuffer_free", avbuffer_free_Callback);
  NAPI_SET_METHOD(exports, "av_dump_disable", av_dump_disable_Callback);
  
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)