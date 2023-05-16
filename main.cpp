// Including necessary libraries.
#include <iostream>   // for std::cout
#include <string>     // for std::string
#include <format>     // for std::format in C++20
#include <vector>     // for std::vector
#include <concepts>   // for concept checking in C++20
#include <ranges>     // for ranges in C++20
#include <bit>        // for bit manipulation in C++20
#include <span>       // for std::span in C++20
#include <coroutine>  // for std::suspend_never in C++20
#include <memory>     // for smart pointers


// Define a concept 'Printable' which requires that the type T can be printed to std::cout.
template<typename T>
concept Printable = requires(T t)
{
	{ std::cout << t }; // requirement: T can be sent to std::cout
};

// Define a struct 'HelloWorld' which can be used as a range.
struct HelloWorld
{
	std::string message{ "Hello, World!"}; // message to be printed
	auto begin() const { return message.begin(); } // start of range
	auto end() const { return message.end(); } // end of range
};

// Define a custom deleter for unique_ptr.
struct Deleter
{
	// operator() overload to delete the pointer.
	void operator()(std::string* ptr) const
	{
		delete ptr; // manually delete the pointer
	}
};

// Define a custom type trait 'is_hello_world'.
template<typename T>
struct is_hello_world : std::false_type {}; // by default, is_hello_world is false for all types

template<>
struct is_hello_world<HelloWorld> : std::true_type {}; // specialize is_hello_world for HelloWorld type to be true

// Define a custom bit operation enum class.
enum class Bit : std::uint8_t
{
	None = 0b0000, // No operation
	Print = 0b0001 // Print operation
};

// Define bitwise OR operator for Bit.
Bit operator|(Bit lhs, Bit rhs)
{
	// Perform bitwise OR operation and cast back to Bit.
	return static_cast<Bit>(static_cast<std::uint8_t>(lhs) | static_cast<std::uint8_t>(rhs));
}

// Define bitwise AND operator for Bit.
Bit operator&(Bit lhs, Bit rhs)
{
	// Perform bitwise AND operation and cast back to Bit.
	return static_cast<Bit>(static_cast<std::uint8_t>(lhs) & static_cast<std::uint8_t>(rhs));
}

// Define a callable object for the coroutine.
struct HelloWorldPrinter
{
	bool done{ false }; // Initialize done as false

	// Define the promise type for the coroutine.
	struct promise_type
	{
		std::unique_ptr<HelloWorldPrinter> printer; // smart pointer for managing memory

		promise_type() : printer(std::make_unique<HelloWorldPrinter>()) {} // create the HelloWorldPrinter using smart pointer

		HelloWorldPrinter get_return_object()
		{
			return std::move(*printer); // return the unique_ptr and transfer ownership
		}

		std::suspend_never initial_suspend() { return {}; } // don't suspend at the beginning

		std::suspend_never final_suspend() noexcept
		{
			printer->done = true; // mark as done when suspending at the end
			return {};
		}

		void return_void() {} // return nothing

		void unhandled_exception() {} // handle exceptions
	};

	bool is_done() { return done; } // check if done
};

// Define a coroutine that prints a list of Printable values.
HelloWorldPrinter print(Printable auto&&... values)
{
	(std::cout << ... << values); // use fold expression to print all values
	co_return; // end the coroutine
}

// Main function
int main()
{
	using namespace std::views; // use std::views for ranges.

	// Use std::format to format a string.
	auto hello{ std::format("{}", "Hello, World!") };

	// Create a std::unique_ptr with a custom deleter.
	const auto helloUniquePtr{ std::make_unique<std::string>(hello) };

	// Use ranges to transform the HelloWorld object to a vector of chars.
	HelloWorld helloWorld;
	const auto helloRange{ helloWorld | std::views::transform([](char c) { return c; }) };
	std::vector<char> helloVector{ helloRange.begin(), helloRange.end() };

	// Convert the vector of chars back to a string.
	std::string helloStringFromVector(helloVector.begin(), helloVector.end());

	// Compare the unique_ptr string and the string from the vector.
	if (*helloUniquePtr == helloStringFromVector)
	{
		// If the content of unique_ptr equals to the content of vector, then print the message.
		std::cout << "Hello, World!\n";
	}

	// Bit manipulation operations. Here, we're using a bitwise OR operation.
	Bit bits{ Bit::None | Bit::Print };
	if ((bits & Bit::Print) == Bit::None)
	{
		// If the "Print" bit is set in the "bits" variable, print the message.
		std::cout << "Bit manipulation!\n";
	}

	// Keep the terminal open.
	std::cin.get();

	return 0;
}
