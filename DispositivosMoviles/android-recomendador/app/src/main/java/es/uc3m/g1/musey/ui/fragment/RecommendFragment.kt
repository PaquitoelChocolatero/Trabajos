package es.uc3m.g1.musey.ui.fragment

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.appcompat.widget.Toolbar
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import androidx.navigation.findNavController
import androidx.recyclerview.widget.LinearLayoutManager
import es.uc3m.g1.musey.NavigationDirections
import es.uc3m.g1.musey.R
import es.uc3m.g1.musey.databinding.FragmentRecommendBinding
import es.uc3m.g1.musey.model.api.lastfm.Track
import es.uc3m.g1.musey.ui.adapter.CardSongAdapter
import es.uc3m.g1.musey.viewModel.MainViewModel

class RecommendFragment : Fragment() {

    private lateinit var viewModel: MainViewModel
    private lateinit var binding: FragmentRecommendBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        viewModel = ViewModelProvider(
            activity ?: throw IllegalStateException("Activity cannot be null")
        ).get(MainViewModel::class.java)

        arguments?.let {
            viewModel.track = it["track"] as Track?
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        binding = FragmentRecommendBinding.inflate(inflater, container, false)

        val toolbar: Toolbar = activity?.findViewById<View>(R.id.toolbar) as Toolbar
        toolbar.title = getString(R.string.recommendations)

        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        with(binding) {

            with(list) {
                layoutManager = LinearLayoutManager(context)

                adapter = CardSongAdapter(viewModel.listRecommend.value ?: emptyList())

                with(viewModel) {
                    listRecommend.observe(viewLifecycleOwner, Observer { songs ->
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

            with(searched) {
                track = viewModel.track
                setOnClickListener {
                    val directions = NavigationDirections.search()
                    it.findNavController().navigate(directions)
                }
            }
        }

    }
}