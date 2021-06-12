package es.uc3m.g1.musey.ui.fragment

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.inputmethod.EditorInfo
import android.widget.TextView.OnEditorActionListener
import android.widget.Toast
import androidx.appcompat.widget.Toolbar
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import androidx.recyclerview.widget.LinearLayoutManager
import es.uc3m.g1.musey.R
import es.uc3m.g1.musey.databinding.FragmentSearchBinding
import es.uc3m.g1.musey.hideKeyboard
import es.uc3m.g1.musey.ui.adapter.CardSongAdapter
import es.uc3m.g1.musey.viewModel.MainViewModel


class SearchFragment : Fragment() {
    private lateinit var viewModel: MainViewModel
    private lateinit var binding: FragmentSearchBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        viewModel = ViewModelProvider(
            activity ?: throw IllegalStateException("Activity cannot be null")
        ).get(MainViewModel::class.java)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        binding = FragmentSearchBinding.inflate(inflater, container, false)

        val toolbar: Toolbar = activity?.findViewById<View>(R.id.toolbar) as Toolbar
        toolbar.title = getString(R.string.search)

        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        with(binding) {

            with(results) {
                layoutManager = LinearLayoutManager(context)

                adapter = CardSongAdapter(viewModel.listSearch.value ?: emptyList())

                with(viewModel) {
                    listSearch.observe(viewLifecycleOwner, Observer { songs ->
                        with(adapter as CardSongAdapter) {
                            values = songs
                            notifyDataSetChanged()
                        }
                    })
                    error.observe(viewLifecycleOwner, Observer { error ->
                        Toast.makeText(context.applicationContext, error, Toast.LENGTH_SHORT).show()
                    })
                }
            }

            button.setOnClickListener {
                viewModel.search = input.text.toString()
                view.hideKeyboard()
            }

            input.setOnEditorActionListener(OnEditorActionListener { _, actionId, _ ->
                if (actionId == EditorInfo.IME_ACTION_SEARCH) {
                    viewModel.search = input.text.toString()
                    view.hideKeyboard()
                    return@OnEditorActionListener true
                }
                false
            })
        }
    }
}