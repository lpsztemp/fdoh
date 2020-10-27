#include <memory>
#include <cstdlib>
#include <cstdio>

#ifndef SBO_VECTOR_H_
#define SBO_VECTOR_H_

template <class VertexType, std::size_t sbo_element_threshold>
class sbo_vector
{
	VertexType m_in_stack[sbo_element_threshold];
	VertexType* m_ptr = m_in_stack;
	std::size_t m_cnt = 0u;
	std::size_t m_cap = sbo_element_threshold;
	static VertexType* alloc_space(std::size_t count, std::size_t* new_cap)
	{
		std::size_t cap = count + sbo_element_threshold - 1;
		cap -= cap % sbo_element_threshold;
		VertexType* pResult = (VertexType*) malloc(cap * sizeof(VertexType));
		if (!pResult)
			throw std::bad_alloc();
		*new_cap = cap;
		return pResult;
	}
	static VertexType* realloc_space(VertexType* old_buf, std::size_t old_count, std::size_t old_cap, std::size_t new_count, std::size_t* new_cap)
	{
		std::size_t cap = new_count + sbo_element_threshold - 1;
		cap -= cap % sbo_element_threshold;
		if (cap <= old_cap)
		{
			*new_cap = old_cap;
			return old_buf;
		}
		auto pResult = (VertexType*) malloc(cap * sizeof(VertexType));
		if (!pResult)
			throw std::bad_alloc();
		std::memcpy(pResult, old_buf, old_count * sizeof(VertexType));
		*new_cap = cap;
		free(old_buf);
		return pResult;
	}
	template <class IteratorBegin, class IteratorEnd>
	sbo_vector(IteratorBegin begin, IteratorEnd end, std::forward_iterator_tag):m_cnt(std::distance(begin, end))
	{
		if (m_cnt > sbo_element_threshold)
			m_ptr = alloc_space(m_cnt, &m_cap);
		std::copy(begin, end, m_ptr);
	}
	template <class IteratorBegin, class IteratorEnd>
	sbo_vector(IteratorBegin begin, IteratorEnd end, std::input_iterator_tag)
	{
		while (begin != end)
		{
			this->emplace_back(*begin);
			++begin;
		}
	}
public:
	sbo_vector() = default;
	sbo_vector(const sbo_vector& right):m_cnt(right.m_cnt)
	{
		if (m_cnt > sbo_element_threshold)
			m_ptr = alloc_space(m_cnt, &m_cap);
		std::memcpy(m_ptr, right.m_ptr, m_cnt * sizeof(VertexType));
	}
	sbo_vector(sbo_vector&& right):m_cnt(right.m_cnt)
	{
		if (right.m_cnt <= sbo_element_threshold)
		{
			std::memcpy(m_ptr, right.m_ptr, m_cnt * sizeof(VertexType));
			right.m_cnt = 0u;
		}else
		{
			m_ptr = right.m_ptr;
			right.m_ptr = right.m_in_stack;
			right.m_cnt = 0u;
			right.m_cap = sbo_element_threshold;
		}
	}
	template <class IteratorBegin, class IteratorEnd>
	inline sbo_vector(IteratorBegin begin, IteratorEnd end):sbo_vector(begin, end, std::common_type_t<
		typename std::iterator_traits<IteratorBegin>::iterator_category, typename std::iterator_traits<IteratorEnd>::iterator_category>()) {}
	~sbo_vector()
	{
		if (m_ptr != m_in_stack)
			free(m_ptr);
	}
	sbo_vector& operator=(const sbo_vector& right)
	{
		if (this == &right)
			return *this;
		if (right.m_cnt > m_cap)
		{
			std::size_t new_cap;
			VertexType* copy_buf = alloc_space(right.m_cnt, &new_cap);
			if (m_ptr != m_in_stack)
				free(m_ptr);
			m_ptr = copy_buf;
			m_cap = new_cap;
		}
		std::memcpy(m_ptr, right.m_ptr, right.m_cnt * sizeof(VertexType));
		m_cnt = right.m_cnt;
		return *this;
	}
	sbo_vector& operator=(sbo_vector&& right)
	{
		if (this == &right)
			return *this;
		if (right.m_cnt > sbo_element_threshold)
		{
			if (m_ptr != m_in_stack)
				free(m_ptr);
			m_ptr = right.m_ptr;
			m_cap = right.m_cap;
			m_cnt = right.m_cnt;
			right.m_ptr = right.m_in_stack;
			right.m_cnt = 0;
			right.m_cap = sbo_element_threshold;
			return *this;
		}
		std::memcpy(m_ptr, right.m_ptr, right.m_cnt * sizeof(VertexType));
		m_cnt = right.m_cnt;
		right.m_cnt = 0;
		return *this;
	}
	const VertexType& operator[](std::size_t off) const noexcept
	{
		return m_ptr[off];
	}
	VertexType& operator[](std::size_t off) noexcept
	{
		return m_ptr[off];
	}
	const VertexType& at(std::size_t off) const noexcept
	{
		return m_ptr[off];
	}
	VertexType& at(std::size_t off) noexcept
	{
		return m_ptr[off];
	}
	const VertexType& front() const noexcept
	{
		return m_ptr[0];
	}
	VertexType& front() noexcept
	{
		return m_ptr[0];
	}
	const VertexType& back() const noexcept
	{
		return m_ptr[m_cnt - 1];
	}
	VertexType& back() noexcept
	{
		return m_ptr[m_cnt - 1];
	}
	typedef VertexType* iterator;
	typedef const VertexType* const_iterator;
	const_iterator begin() const noexcept
	{
		return m_ptr;
	}
	const_iterator cbegin() const noexcept
	{
		return m_ptr;
	}
	iterator begin() noexcept
	{
		return m_ptr;
	}
	const_iterator end() const noexcept
	{
		return m_ptr + m_cnt;
	}
	const_iterator cend() const noexcept
	{
		return m_ptr + m_cnt;
	}
	iterator end() noexcept
	{
		return m_ptr + m_cnt;
	}
	std::size_t size() const noexcept
	{
		return m_cnt;
	}
	std::size_t capacity() const noexcept
	{
		return m_cap;
	}
	bool empty() const noexcept
	{
		return m_cnt == 0u;
	}
	void reserve(std::size_t new_cap)
	{
		if (new_cap <= m_cap)
			return;
		if (m_ptr != m_in_stack)
			m_ptr = realloc_space(m_ptr, m_cnt, m_cap, new_cap, &m_cap);
		else
		{
			m_ptr = alloc_space(new_cap, &m_cap);
			std::memcpy(m_ptr, m_in_stack, m_cnt * sizeof(VertexType));
		}
	}
	template <class ... VertexConstructionParams>
	auto emplace_back(VertexConstructionParams&& ... params) -> std::enable_if_t<std::is_constructible<VertexType, VertexConstructionParams&&... >::value, iterator>
	{
		std::size_t new_count = m_cnt + 1;
		if (new_count > m_cap)
		{
			if (m_ptr != m_in_stack)
				m_ptr = realloc_space(m_ptr, m_cnt, m_cap, new_count, &m_cap);
			else
			{
				m_ptr = alloc_space(new_count, &m_cap);
				std::memcpy(m_ptr, m_in_stack, m_cnt * sizeof(VertexType));
			}
		}
		auto buf = m_ptr + m_cnt;
		new (buf) VertexType(std::forward<VertexConstructionParams>(params)...);
		m_cnt = new_count;
		return buf;
	}
};

#endif //SBO_VECTOR_H_
