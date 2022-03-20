#pragma once

template<typename T>
struct ResourceShared
{
	static_assert(std::is_copy_assignable_v<T>);
	static_assert(std::is_constructible_v<T>);
	static_assert(std::is_copy_constructible_v<T>);
	static_assert(std::is_move_constructible_v<T>);
	static_assert(std::is_move_assignable_v<T>);


	T* data;
	std::string id;
	std::string path;
	std::atomic<int> refs{ 1 };

	ResourceShared(T* data,
		std::string path)
	{
		data = data;
		path = path;
		refs = 1;
	}

	ResourceShared(ResourceShared& other) = delete;

	ResourceShared(ResourceShared&& other) = delete;

	ResourceShared(const ResourceShared&& other) = delete;

	~ResourceShared()
	{
		delete data;
	}
};

template<typename T>
struct Resource
{
	static_assert(std::is_copy_assignable_v<T>);
	static_assert(std::is_constructible_v<T>);
	static_assert(std::is_copy_constructible_v<T>);
	static_assert(std::is_move_constructible_v<T>);
	static_assert(std::is_move_assignable_v<T>);

	ResourceShared<T>* resource;

	Resource() {};

	Resource(ResourceShared<T>* res)
	{
		resource = res;
		resource->refs++;
	}

	Resource(Resource& other)// copy constructor
	{
		resource = other.resource;
		resource->refs++;
	}

	Resource(const Resource& other) // copy constructor
	{
		resource = other.resource;
		resource->refs++;
	}

	Resource(Resource&& other) noexcept // move constructor
	{
		resource = other.resource;
		resource->refs++;
	}
	
	Resource(const Resource&& other) noexcept // move constructor
	{
		resource = other.resource;
		resource->refs++;
	}

	Resource& operator=(const Resource& other) // copy assignment
	{
		resource = other.resource;
		resource->refs++;
		return *this;
	}

	Resource& operator=(Resource&& other) noexcept // move assignment
	{
		resource = other.resource;
		resource->refs++;
		return *this;
	}

	~Resource()
	{
		resource->refs--;
	}

	T* operator-> ()
	{
		return resource->data;
	}
};

