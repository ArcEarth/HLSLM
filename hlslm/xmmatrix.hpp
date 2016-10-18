#pragma once
#include "traits.hpp"
#include "xmvector.hpp"

#ifndef _HLSLM_MATRIX_DEFAULT_COLUMN_MAJOR
#define _HLSLM_MATRIX_DEFAULT_COLUMN_MAJOR 0
#endif

namespace DirectX
{
	namespace hlsl
	{
		namespace detail
		{
			template <typename _Scalar, size_t _Rows, size_t _Cols, MatrixMajorEnum _Major = 0>
			struct alignas(16) matrix_storage_base
			{
				static constexpr size_t Size = _Rows * _Cols;
				static constexpr size_t Rows = _Rows;
				static constexpr size_t Cols = _Cols;
				static constexpr MatrixMajorEnum Major = _Major;

				static constexpr size_t dimension(size_t dim) {
					return dim > 1 ? 0 : dim ^ Major == 1 ? Rows : Cols;
				}

				using Scalar = _Scalar;
				using InnerVectorType = xmvector<Scalar, dimension(0)>;
				using OuterVectorType = xmvector<Scalar, dimension(1)>;

				InnerVectorType m[dimension(1)];
			};

			template <typename _TDerived, size_t _Rows, size_t _Cols>
			struct matrix_element_access_operator
			{
			};

			template <typename _TDerived>
			struct matrix_arthmatic_operator_base : public matrix_element_access_operator<_TDerived, _TDerived::Rows, _TDerived::Cols>
			{
			};

			template <typename _TDerived, typename _TScalar>
			struct matrix_logic_bitwise_operator_base : public matrix_arthmatic_operator_base<_TDerived>
			{
			};

			template <typename _TDerived>
			struct matrix_operator_base : public matrix_logic_bitwise_operator_base<_TDerived, typename _TDerived::Scalar>
			{
			};

		}

		namespace detail
		{
			template <typename _TMat>
			struct matrix_dialog_expr;

			template <typename _TMat>
			struct matrix_transpose_expr;

			template <typename _TMat, index_t _Row>
			struct matrix_row_expr;

			template <typename _TMat, index_t _Col>
			struct matrix_col_expr;

			template <typename _TMat, index_t _Row, size_t _Col>
			struct matrix_element_expr;

			template <typename _TMat, index_t _Row, size_t _Col, index_t _BlockRows, size_t _BlockCols>
			struct matrix_block_expr;

			template <typename _TOperator, typename _TLhs, typename _TRhs>
			struct matrix_arthmatic_expr;

			template <typename _TMat>
			struct transposed;

			template <typename _TMat, index_t _Row, size_t _Col, index_t _BlockRows, size_t _BlockCols>
			struct blocked;

			#pragma region ExpressionCompositionRules
			template <typename _TMat>
			struct transposed {
				using type = matrix_transpose_expr<_TMat>;
			};

			// Channing rule of Transpose Operator
			template <typename _TMat>
			struct transposed<matrix_transpose_expr<_TMat>> {
				using type = _TMat;
			};

			template <typename _TMat, index_t _Row, size_t _Col, index_t _BlockRows, size_t _BlockCols>
			struct blocked;

			template <typename _TMat, index_t _Row, size_t _Col, index_t _BlockRows, size_t _BlockCols>
			struct blocked
			{
				using type = matrix_block_expr<_TMat, _Row, _Col, _BlockRows, _BlockCols>;
			};

			// Chaining Rule of Block & Transpose Operator
			// Commuter of Operator Transpose & Block
			// Always commute transpose_expr to outter part, so that it can be eliminate when transpose again
			template <typename _TMat, index_t _Row, size_t _Col, index_t _BlockRows, size_t _BlockCols>
			struct blocked<matrix_transpose_expr<_TMat>, _Row, _Col, _BlockRows, _BlockCols>
			{
				using type = typename transposed<typename blocked<_TMat, _Col, _Row, _BlockCols, _BlockRows>::type>::type;
			};

			// Degrade Block Expr to Element Expr when size equals 1
			template <typename _TMat, index_t _Row, size_t _Col>
			struct blocked<_TMat, _Row, _Col, 1, 1>
			{
				using type = matrix_element_expr<_TMat, _Row, _Col>;
			};

			// Chaining rule of Block Operator
			template <typename _TMat, index_t _SRow, size_t _SCol, index_t _SBRows, size_t _SBCols, index_t _Row, size_t _Col, index_t _BRows, size_t _BCols>
			struct blocked<matrix_block_expr<_TMat, _SRow, _SCol, _SBRows, _SBCols>, _Row, _Col, _BRows, _BCols>
			{
				using type = typename blocked<_TMat, _Row + _SRow, _Col + _SCol, _BRows, _BCols>::type;
			};
			#pragma endregion

			template <typename _TMat>
			using transposed_t = typename transposed<_TMat>::type;

			template <typename _TMat, index_t _Row, size_t _Col, index_t _BlockRows, size_t _BlockCols>
			using blocked_t = typename blocked<_TMat, _Row, _Col, _BRows, _BCols>::type;
		}

		template <typename _TDerived, typename _TResult>
		struct matrix_expr : public detail::matrix_operator_base<_TDerived> {
			using result_t = _TResult;
			using scalar_t = typename result_t::Scalar;
			static constexpr Rows = result_t::Rows;
			static constexpr Rows = result_t::Cols;

			// An matrix expression cannot be instancinated
			// One should awalys acquire it by reinterpret_cast
			// And hold it by reference
			xm_matrix_expr() = delete;

			auto&& eval() const {
				return _TDerived::eval();
			}

			template <typename _TSrcExpr>
			void
				assign(_TSrcExpr&& src_expr) {
				static_assert(traits::is_assignable<result_t, typename _TSrcExpr::result_t>::value, "Assignment expression type not agree.");
				_TDerived::assign(std::forward(src_expr));
			}

			constexpr size_t rows() const { return _TResult::Rows; }
			constexpr size_t cols() const { return _TResult::Cols; }

			template <_TExpr>
			inline const _TExpr& as_expr() const { return reinterpret_cast<const _TExpr&>(*this);}
			template <_TExpr>
			inline _TExpr& as_expr() { return reinterpret_cast<_TExpr&>(*this); }

			const auto& transpose() const { return this->as_expr<detail::transposed_t<_TDerived>>(); }

			template <index_t _row, index_t _col, size_t _block_rows, size_t _block_cols>
			const auto& block() const { return this->as_expr<detail::blocked_t<_TDerived,_row, _col, _block_rows, _block_cols>>(); }

			// Special block
			template <index_t _row, index_t _col>
			auto& elem() const { this->block<_row, _col, 1, 1>() }
			template <index_t _row>
			auto& row() const { return this->block<_row, 0, 1, Cols>(); }
			template <index_t _col>
			auto& col() const { return this->block<0, _col, Rows, 1>(); }
			template <index_t _cols=1>
			auto& left_cols() const { return this->block<0, 0, Rows, _cols>(); }
			template <index_t _cols=1>
			auto& right_cols() const { return this->block<0, Cols - _cols, Rows, _cols>(); }
			template <index_t _rows=1>
			auto& top_rows() const { return this->block<0, 0, _rows, Cols>(); }
			template <index_t _rows=1>
			auto& bottom_rows() const { return this->block<Rows - _rows, 0, _rows, Cols>(); }
		};

		namespace detail
		{
			template <typename _Scalar, size_t _Rows, size_t _Cols, MatrixMajorEnum _Major = 0>
			inline void XM_CALLCONV do_transpose(const matrix_storage_base<_Scalar, Rows, Cols, _Major>& src, matrix_storage_base<_Scalar, Rows, Cols, _Major>& dest)
			{
				
			}


			template <typename _TMat>
			struct transposed : public matrix_expr<transposed<_TMat>, xmmatrix<typename _TMat::Scalar, _TMat::Cols, _TMat::Rows>>
			{
				using result_t = xmmatrix<typename _TMat::Scalar, _TMat::Cols, _TMat::Rows>;
				_TMat _operand;
				result_t eval() const {
					result_t val;
					auto&& src = _operand.eval();
					do_transpose(src, static_cast<typename result_t::storage_t&>(val));
					return val;
				}
			};
		}

		template <typename _T, size_t _Rows, size_t _Cols, MatrixMajorEnum _Major = 0>
		struct xmmatrix
		{
			static constexpr size_t Size = _Rows * _Cols;
			static constexpr size_t Rows = _Rows;
			static constexpr size_t Cols = _Cols;
			static constexpr size_t Rank = (Cols < Rows) ? Cols : Rows;
			using Scalar = _T;
			using RowVectorType = xmvector<Scalar, Cols>;
			using ColVectorType = xmvector<Scalar, Rows>;
			using DialogVectorType = xmvector<Scalar, Rank>;

			RowVectorType r[Rows];

			// Row accessers
			template <index_t _row>
			RowVectorType row() const { return r[_row]; }
			template <index_t _row>
			RowVectorType& row() { return r[_row]; }

			// Column accessers
			template <index_t _col>
			ColVectorType get_col() const { return r[_col]; }
			template <index_t _col, class rhs_t>
			std::enable_if_t<traits::is_assignable<ColVectorType, rhs_t>::value>
				set_col(const rhs_t& value) { return r[_col]; }

			// element accessers
			template <index_t _row, index_t _col>
			Scalar get() const;
			template <index_t _row, index_t _col>
			void set(Scalar value);



		};

		using xmmatrix4f = xmmatrix<float, 4, 4>;
		using xmmatrix3f = xmmatrix<float, 3, 3>;
		using xmmatrix4x4f = xmmatrix<float, 4, 4>;
		using xmmatrix3x3f = xmmatrix<float, 3, 3>;
		using xmmatrix2x4f = xmmatrix<float, 2, 4>;
		using xmmatrix3x4f = xmmatrix<float, 2, 4>;
	}
}