#include "mu-core-tests.inl"

#include "mu-core/Paths.h"
#include "mu-core/String.h"

namespace mu_core_tests_string {
	using namespace mu;

	TEST_CLASS(GetExtensionTests) {
public:
	TEST_METHOD(Filename) {
		String s{ "foo.txt" };
		auto ext = paths::GetExtension(Range(s));
		Assert::IsFalse(ext.IsEmpty());
		String ext_s{ ext };
		Assert::AreEqual<String>("txt", ext_s);
	}
	TEST_METHOD(MultipleExtensions) {
		String s{ "foo.txt.zip" };
		auto ext = paths::GetExtension(Range(s));
		Assert::IsFalse(ext.IsEmpty());
		String ext_s{ ext };
		Assert::AreEqual<String>("zip", ext_s);
	}
	TEST_METHOD(CurrentDir) {
		String s{ "." };
		auto ext = paths::GetExtension(Range(s));
		Assert::IsTrue(ext.IsEmpty());
	}
	TEST_METHOD(ParentDir) {
		String s{ ".." };
		auto ext = paths::GetExtension(Range(s));
		Assert::IsTrue(ext.IsEmpty());
	}

	TEST_METHOD(RelativePathPlusCurrentDir) {
		String s{ "../Foo/Bar/." };
		auto ext = paths::GetExtension(Range(s));
		Assert::IsTrue(ext.IsEmpty());
	}
	TEST_METHOD(RelativePathPlusParentDir) {
		String s{ "/Foo/Bar/.." };
		auto ext = paths::GetExtension(Range(s));
		Assert::IsTrue(ext.IsEmpty());
	}

	TEST_METHOD(RelativePathWithNoFilename) {
		String s{ "../Foo/Bar/Baz/" };
		auto ext = paths::GetExtension(Range(s));
		Assert::IsTrue(ext.IsEmpty());
	}

	TEST_METHOD(RelativePathFilenameNoExtension) {
		String s{ "../Foo/Bar/Baz" };
		auto ext = paths::GetExtension(Range(s));
		Assert::IsTrue(ext.IsEmpty());
	}

	TEST_METHOD(RelativePathFilenameWithExtension) {
		String s{ "../Foo/Bar/Baz.txt" };
		auto ext = paths::GetExtension(Range(s));
		Assert::IsFalse(ext.IsEmpty());
		String ext_s{ ext };
		Assert::AreEqual<String>("txt", ext_s);
	}


	};

	TEST_CLASS(GetFilenameTests) {
public:
	TEST_METHOD(Filename) {
		String s{ "foo.txt" };
		auto fn = paths::GetFilename(Range(s));
		Assert::IsFalse(fn.IsEmpty());
		String fn_s{ fn };
		Assert::AreEqual<String>(s, fn_s);
	}
	TEST_METHOD(RelativePathAndFilename) {
		String s{ "../bar/baz/foo.txt" };
		auto fn = paths::GetFilename(Range(s));
		Assert::IsFalse(fn.IsEmpty());
		String fn_s{ fn };
		Assert::AreEqual<String>("foo.txt", fn_s);
	}
	TEST_METHOD(RootPathAndFilename) {
		String s{ "/foo.txt" };
		auto fn = paths::GetFilename(Range(s));
		Assert::IsFalse(fn.IsEmpty());
		String fn_s{ fn };
		Assert::AreEqual<String>("foo.txt", fn_s);
	}

	TEST_METHOD(MultipleExtensions) {
		String s{ "foo.txt.zip" };
		auto fn = paths::GetFilename(Range(s));
		Assert::IsFalse(fn.IsEmpty());
		String fn_s{ fn };
		Assert::AreEqual<String>(s, fn_s);
	}
	};
}