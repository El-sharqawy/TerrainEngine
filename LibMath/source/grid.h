#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <cassert>
#include <memory>
#include <algorithm>
#include <stdexcept>

#include "../../LibGL/source/utils.h"

// Max Size for Grid
#define MAX_SIZE 1 << 30

template <typename T>
class CGrid
{
public:
	CGrid() = default;

	CGrid(GLint Cols, GLint Rows)
	{
		m_iCols = 0;
		m_iRows = 0;
		m_iGridSize = 0;
		m_pGrid = nullptr;

		InitGrid(Cols, Rows);
	}

	CGrid(GLint Cols, GLint Rows, GLint InitValue)
	{
		m_iCols = 0;
		m_iRows = 0;
		m_iGridSize = 0;
		m_pGrid = nullptr;

		InitGrid(Cols, Rows, InitValue);
	}


	~CGrid()
	{
		Destroy();
	}

	// Initialize Grid With Value
	void InitGrid(GLint Cols, GLint Rows, T InitValue)
	{
		InitGrid(Cols, Rows);

		for (GLint i = 0; i < m_iGridSize; i++)
		{
			m_pGrid[i] = InitValue;
		}


		sys_log("CGrid::InitGrid Grid Data, Rows: %u, Cols: %u, GridSize: %u", m_iRows, m_iCols, m_iGridSize);
	}

	// Initialize Grid
	void InitGrid(GLint Cols, GLint Rows)
	{
		m_iCols = Cols;
		m_iRows = Rows;
		m_iGridSize = Cols * Rows;

		safe_free(m_pGrid);

		//m_pGrid = std::make_unique<T[]>(static_cast<size_t>(Cols) * static_cast<size_t>(Rows) * sizeof(T));
		m_pGrid = (T*)malloc(Cols * Rows * sizeof(T));
	}

	// not sure how this gonna work, test it.
	void InitGrid(GLint Cols, GLint Rows, void* pData)
	{
		m_iCols = Cols;
		m_iRows = Rows;
		m_iGridSize = Cols * Rows;

		safe_free(m_pGrid);

		m_pGrid = (T*)pData;

		sys_log("CGrid::InitGrid Grid Data, Rows: %u, Cols: %u, GridSize: %u", m_iRows, m_iCols, m_iGridSize);
	}

	// is it really needed?
	void Destroy()
	{
		safe_free(m_pGrid);
	}

	size_t CalculateIndex(GLint Col, GLint Row) const
	{
#if defined(_DEBUG)
		if (Col > m_iCols || Row > m_iRows)
		{
			throw std::out_of_range("Invalid column or row index");
		}
#endif
		size_t Index = static_cast<size_t>(Row * m_iCols + Col);
		return (Index);
	}

	/* Get the Grid Size */
	GLint GetSize() const
	{
		return (m_iGridSize);
	}

	T* GetAddr(GLint Col, GLint Row) const
	{
		size_t Index = CalculateIndex(Col, Row);
		return &(m_pGrid[Index]);
	}

	T* GetBaseAddr() const
	{
		return m_pGrid;
	}

	GLint GetSizeByBytes() const
	{
		return (GetSize() * sizeof(T));
	}
	
	T& At(GLint Col, GLint Row)
	{
		size_t Index = CalculateIndex(Col, Row);
		return (m_pGrid[Index]);
	}

	void Set(GLint Index, const T& Value)
	{
#if defined(_DEBUG)
		if (Index >= m_iGridSize)
		{
			throw std::out_of_range("Invalid Index");
		}
#endif
		m_pGrid[Index] = Value;
	}

	void Set(GLint Col, GLint Row, const T& Value)
	{
		*GetAddr(Col, Row) = Value;
	}

	const T& Get(GLint Col, GLint Row) const
	{
		return (*GetAddr(Col, Row));
	}

	const T& Get(GLint Index) const
	{
#if defined(_DEBUG)
		if (Index >= m_iGridSize)
		{
			throw std::out_of_range("Invalid Index");
		}
#endif

		return (m_pGrid[Index]);
	}

	void GetMinMax(T& Min, T& Max)
	{
		Min = Max = m_pGrid[0];

		for (GLint i = 0; i < m_iGridSize; i++)
		{
			if (m_pGrid[i] < Min)
			{
				Min = m_pGrid[i];
			}
			if (m_pGrid[i] > Max)
			{
				Max = m_pGrid[i];
			}
		}
	}

	/**
	 * Normalize the grid values to a specified range.
	 *
	 * This function normalizes the values in the grid (`m_pGrid`) to a new range defined
	 * by `MinRange` and `MaxRange`. The normalization is done by first calculating the
	 * minimum and maximum values in the grid, then mapping these values to the desired
	 * range while preserving the relative scale between grid values.
	 *
	 * The algorithm performs the following steps:
	 * 1. Retrieves the minimum (`Min`) and maximum (`Max`) values from the grid using
	 *    the `GetMinMax` function.
	 * 2. If the maximum value is less than or equal to the minimum value, no normalization
	 *    is performed and the function returns early.
	 * 3. Calculates the range between the minimum and maximum values (`MinMaxDelta`).
	 * 4. Calculates the target range between `MinRange` and `MaxRange` (`MinMaxRange`).
	 * 5. Iterates through each element of the grid and normalizes the value using the
	 *    formula:
	 *      ((value - Min) / MinMaxDelta) * MinMaxRange + MinRange
	 *    This formula maps the grid value from the original range `[Min, Max]` to the
	 *    new range `[MinRange, MaxRange]`.
	 *
	 * @param MinRange: The lower bound of the target range.
	 * @param MaxRange: The upper bound of the target range.
	 *
	 * @return: None
	 */
	void Normalize(T MinRange, T MaxRange)
	{
		T Min, Max;

		GetMinMax(Min, Max);

		if (Max <= Min)
		{
			return;
		}

		T MinMaxDelta = Max - Min;
		T MinMaxRange = MaxRange - MinRange;

		for (GLint i = 0; i < m_iGridSize; i++)
		{
			m_pGrid[i] = ((m_pGrid[i] - Min) / MinMaxDelta) * MinMaxRange + MinRange;
		}
	}

	void PrintFloat()
	{
		for (GLint y = 0; y < m_iRows; y++)
		{
			printf("%d: ", y);
			for (GLint x = 0; x < m_iCols; x++)
			{
				float f = (float)m_pGrid[y * m_iCols + x];
				printf("%.6f", f);
			}
			printf("\n");
		}
	}

	GLint GetWidth()
	{
		return (m_iCols);
	}

	GLint GetDepth()
	{
		return (m_iRows);
	}

private:
	GLint m_iCols;
	GLint m_iRows;
	GLint m_iGridSize;

	//std::unique_ptr<T[]> m_pGrid;
	T* m_pGrid;
};