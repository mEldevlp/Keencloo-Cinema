#include "Test.h"
#include "Keencloo.h"

int main(int argc, char** argv)
{
	Test test;
	TEST_CASE_ADD(test, "Metadata Extracting", [&test]()
	{
		std::string file_path = "C:\\Users\\Daniel\\Desktop\\projects_cpp\\Keencloo-Cinema\\videos\\test.mp4";
		MetaDataVideo metadata = extract_video_metadata(file_path.c_str());
		
		test.assertNotNull(metadata.frame_data.data.data());
		test.assertEquals(640, metadata.frame_data.resolution.width);
		test.assertEquals(360, metadata.frame_data.resolution.height);
		test.assertEquals(30.00, metadata.fps);
		test.assertEquals(file_path, std::string(metadata.file_path));
		test.assertNotEquals(std::string("video_for_unit_test.mp4"), metadata.file_name);
		test.assertEquals(std::string("video_for_unit_test"), metadata.file_name);
		test.assertEquals(18168034ll, metadata.file_size);
		test.assertEquals(213ll, metadata.duration / 1000000ll);
			
		return test.getCaseResult();
	});

	TEST_CASE_ADD(test, "Video file", [&test]()
	{
		MetaDataVideo metadata = extract_video_metadata("C:\\Users\\Daniel\\Desktop\\projects_cpp\\Keencloo-Cinema\\videos\\test.mp4");
		VideoFile video;

		video.preview = QImage(metadata.frame_data.data.data(),
			metadata.frame_data.resolution.width,
			metadata.frame_data.resolution.height,
			metadata.frame_data.resolution.width * metadata.frame_data.channels,
			QImage::Format_RGB888);
			
		test.assertNotNull(video.preview.data_ptr());

		video.fileSize = video.convertToByte(metadata.file_size);
		test.assertEquals(std::string("17.32 MB"), video.fileSize.toStdString());

		video.videoBitRate = video.convertToByte(metadata.video_bit_rate, true) + "ps";
		test.assertEquals(std::string("546 KBps"), video.videoBitRate.toStdString());

		video.audioBitRate = video.convertToByte(metadata.audio_bit_rate, true) + "ps";
		test.assertEquals(std::string("128 KBps"), video.audioBitRate.toStdString());

		// Random tests
		test.assertEquals(std::string("0.0 B"), video.convertToByte(0ull).toStdString());
		test.assertEquals(std::string("1.0 B"), video.convertToByte(1ull).toStdString());
		test.assertEquals(std::string("1.0 KB"), video.convertToByte(1024ull).toStdString());
		test.assertEquals(std::string("1.0 MB"), video.convertToByte(1024ull * 1024ull).toStdString());
		test.assertEquals(std::string("1.0 GB"), video.convertToByte(1024ull * 1024ull * 1024ull).toStdString());
		test.assertEquals(std::string("10.0 GB"), video.convertToByte(1024ull * 1024ull * 1024ull * 10ull).toStdString());
		test.assertEquals(std::string("100.0 GB"), video.convertToByte(1024ull * 1024ull * 1024ull * 100ull).toStdString());

		return test.getCaseResult();
	});

	test.run();

	std::system("pause");
	return 0;
}
